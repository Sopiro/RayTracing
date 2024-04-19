#pragma once

#include "parallel.h"

namespace bulbit
{

template <typename T>
class AsyncJob : public ParallelJob
{
public:
    AsyncJob(std::function<T(void)> f)
        : func(std::move(f))
    {
    }

    virtual bool HaveWork() const override
    {
        return !started;
    }

    virtual void RunStep(std::unique_lock<std::mutex>* lock) override
    {
        thread_pool->RemoveJob(this);
        started = true;
        lock->unlock();

        T res = func();
        std::unique_lock<std::mutex> l(mutex);
        result = res;
        cv.notify_all();
    }

    bool IsReady() const
    {
        std::lock_guard<std::mutex> l(mutex);
        return result.has_value();
    }

    T GetResult()
    {
        Wait();
        std::lock_guard<std::mutex> l(mutex);
        return result.value();
    }

    void Wait()
    {
        while (!IsReady())
        {
            if (thread_pool->WorkOrReturn() == false)
            {
                // Other thread is running this job
                break;
            }
        }

        std::unique_lock<std::mutex> lock(mutex);
        while (!result.has_value())
        {
            cv.wait(lock);
        }
    }

    void DoWork()
    {
        T res = func();

        std::unique_lock<std::mutex> lock(mutex);
        result = res;
        cv.notify_all();
    }

private:
    std::function<T(void)> func;
    std::optional<T> result;

    mutable std::mutex mutex;
    std::condition_variable cv;

    bool started = false;
};

template <typename F, typename... Args>
inline auto RunAsync(ThreadPool* thread_pool, F func, Args&&... args)
{
    auto fvoid = std::bind(func, std::forward<Args>(args)...);
    using R = std::invoke_result_t<F, Args...>;
    auto job = std::make_unique<AsyncJob<R>>(std::move(fvoid));

    std::unique_lock<std::mutex> lock;
    if (!thread_pool)
    {
        job->DoWork();
    }
    else
    {
        lock = thread_pool->AddJob(job.get());
    }

    return job;
}

template <typename F, typename... Args>
inline auto RunAsync(F func, Args&&... args)
{
    return RunAsync(ThreadPool::global_thread_pool.get(), func, std::forward<Args>(args)...);
}

} // namespace bulbit
