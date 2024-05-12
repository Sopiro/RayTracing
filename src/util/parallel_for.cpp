#include "bulbit/parallel_for.h"

namespace bulbit
{

void ParallelForLoop::RunStep(std::unique_lock<std::mutex>* lock)
{
    int32 index_begin = next_index;
    int32 index_end = std::min(index_begin + chunk_size, end_index);
    next_index = index_end;

    // Remove job from list if all work has been started
    if (HaveWork() == false)
    {
        thread_pool->RemoveJob(this);
    }

    // Release lock and execute loop iterations in [begin, end)
    lock->unlock();
    func(index_begin, index_end);
}

void ParallelFor(int32 begin, int32 end, std::function<void(int32, int32)> func, ThreadPool* thread_pool)
{
    if (begin == end)
    {
        return;
    }

    if (!thread_pool)
    {
        func(begin, end);
        return;
    }

    // Compute chunk size for parallel loop
    const int32 balance = 8;
    int32 chunk_size = std::max<int32>(1, (end - begin) / (balance * int32(thread_pool->WorkerCount())));

    // It's safe to allocate loop on the stack
    // Because this ParallelFor() call does not return until all work for the loop is done.
    ParallelForLoop loop(begin, end, chunk_size, std::move(func));
    std::unique_lock<std::mutex> lock = thread_pool->AddJob(&loop);

    // Current thread also work on the job
    while (!loop.Finished())
    {
        thread_pool->WorkOrWait(&lock);
    }
}

} // namespace bulbit
