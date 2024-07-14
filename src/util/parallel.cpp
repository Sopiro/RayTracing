#include "bulbit/parallel.h"
#include "bulbit/parallel_for.h"

#include <latch>

namespace bulbit
{

ThreadPool::ThreadPool(int32 worker_count)
{
    // Calling thread also participates in executing parallel work,
    // so we launches one fewer than the requested number of threads.
    for (int32 i = 0; i < worker_count - 1; ++i)
    {
        threads.emplace_back(&ThreadPool::Worker, this);
    }
}

ThreadPool::~ThreadPool()
{
    if (threads.empty())
    {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(mutex);
        shutdown = true;
        job_list_condition.notify_all();
    }

    for (std::thread& thread : threads)
    {
        thread.join();
    }
}

void ThreadPool::Worker()
{
    std::unique_lock<std::mutex> lock(mutex);

    while (!shutdown)
    {
        WorkOrWait(&lock);
    }
}

void ThreadPool::WorkOrWait(std::unique_lock<std::mutex>* lock)
{
    assert(lock->owns_lock() == true);

    // Pick one job that still has work left
    ParallelJob* job = job_list;
    while (job && job->HaveWork() == false)
    {
        job = job->next;
    }

    if (job)
    {
        // Execute work for this job
        job->active_workers++;
        job->RunStep(lock);

        // Detach from this job
        assert(lock->owns_lock() == false);
        lock->lock();
        job->active_workers--;

        // If the job is completed,
        // we must signal condition variable for the thread that initially add the work.
        // That initial thread may be waiting on the condition variable for other threads to finish their work on the job.
        if (job->Finished())
        {
            job_list_condition.notify_all();
        }
    }
    else
    {
        // Wait for new work to arrive or the job to finish
        job_list_condition.wait(*lock);
    }
}

bool ThreadPool::WorkOrReturn()
{
    // Return false if we do nothing

    std::unique_lock<std::mutex> lock(mutex);

    ParallelJob* job = job_list;
    while (job && job->HaveWork() == false)
    {
        job = job->next;
    }

    if (job == nullptr)
    {
        return false;
    }

    job->active_workers++;
    job->RunStep(&lock);

    assert(lock.owns_lock() == false);
    lock.lock();
    job->active_workers--;

    if (job->Finished())
    {
        job_list_condition.notify_all();
    }

    return true;
}

std::unique_lock<std::mutex> ThreadPool::AddJob(ParallelJob* job)
{
    job->thread_pool = this;

    std::unique_lock<std::mutex> lock(mutex);

    // Link job to head of list
    {
        if (job_list)
        {
            job_list->prev = job;
        }

        job->next = job_list;
        job_list = job;
    }

    // Notify to all workers
    job_list_condition.notify_all();

    return lock;
}

void ThreadPool::RemoveJob(ParallelJob* job)
{
    // The lock must be held before calling this function
    if (job->prev)
    {
        job->prev->next = job->next;
    }
    else
    {
        job_list = job->next;
    }

    if (job->next)
    {
        job->next->prev = job->prev;
    }
}

void ThreadPool::ForEachThread(std::function<void(void)> func)
{
    int32 worker_count = WorkerCount();
    std::latch latch(worker_count);

    ParallelFor(
        0, worker_count,
        [&](int32) {
            func();
            latch.arrive_and_wait();
        },
        this
    );
}

} // namespace bulbit
