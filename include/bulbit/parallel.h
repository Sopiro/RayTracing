#pragma once

#include "common.h"
#include <mutex>
#include <optional>
#include <thread>

namespace bulbit
{

class ParallelJob
{
public:
    virtual ~ParallelJob() = default;

    virtual bool HaveWork() const = 0;
    virtual void RunStep(std::unique_lock<std::mutex>* lock) = 0;

    bool Finished() const
    {
        return !HaveWork() && active_workers == 0;
    }

private:
    friend class ThreadPool;

    // Active threads working on this job
    int32 active_workers = 0;

    // Links
    ParallelJob* prev = nullptr;
    ParallelJob* next = nullptr;
};

class ThreadPool
{
public:
    explicit ThreadPool(int32 thread_count);
    ~ThreadPool();

    void WorkOrWait(std::unique_lock<std::mutex>* lock, bool is_enqueing_thread);
    bool WorkOrReturn();

    std::unique_lock<std::mutex> AddJob(ParallelJob* job);
    void RemoveJob(ParallelJob* job);

private:
    void Worker();

    bool shutdown = false;

    std::vector<std::thread> threads;
    std::mutex mutex;
    std::condition_variable job_list_condition;

    ParallelJob* job_list = nullptr;
};

} // namespace bulbit
