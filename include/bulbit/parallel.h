#pragma once

#include "common.h"

#include <condition_variable>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <thread>

namespace bulbit
{

class ThreadPool;

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

protected:
    friend class ThreadPool;
    ThreadPool* thread_pool;

private:
    // Active threads working on this job
    int32 active_workers = 0;

    // Links
    ParallelJob* prev = nullptr;
    ParallelJob* next = nullptr;
};

class ThreadPool
{
public:
    inline static std::unique_ptr<ThreadPool> global_thread_pool = nullptr;

    explicit ThreadPool(int32 worker_count);
    ~ThreadPool();

    void WorkOrWait(std::unique_lock<std::mutex>* lock);
    bool WorkOrReturn();

    std::unique_lock<std::mutex> AddJob(ParallelJob* job);
    void RemoveJob(ParallelJob* job);

    void ForEachThread(std::function<void(void)> func);

    int32 WorkerCount() const
    {
        return int32(threads.size() + 1);
    }

private:
    void Worker();

    bool shutdown = false;

    std::vector<std::thread> threads;
    std::mutex mutex;
    std::condition_variable job_list_condition;

    ParallelJob* job_list = nullptr;
};

template <typename T>
class ThreadLocal
{
public:
    ThreadLocal()
        : hash_table{ 4 * std::thread::hardware_concurrency() }
        , createFcn{ []() { return T(); } }
    {
    }

    ThreadLocal(std::function<T(void)> createFcn)
        : hash_table{ 4 * std::thread::hardware_concurrency() }
        , createFcn{ std::move(createFcn) }
    {
    }

    T& Get();

    void ForEach(std::function<void(std::thread::id tid, T& value)>&& callback);

private:
    struct Entry
    {
        std::thread::id tid;
        T value;
    };

    std::shared_mutex mutex;
    std::vector<std::optional<Entry>> hash_table;
    std::function<T(void)> createFcn;
};

template <typename T>
inline T& ThreadLocal<T>::Get()
{
    const std::thread::id tid = std::this_thread::get_id();
    size_t hash = std::hash<std::thread::id>()(tid);
    hash %= hash_table.size();

    int32 step = 1;
    int32 tries = 0;
    BulbitNotUsed(tries);

    mutex.lock_shared();
    while (true)
    {
        BulbitAssert(size_t(tries) < hash_table.size());

        if (hash_table[hash].has_value())
        {
            if (hash_table[hash]->tid == tid)
            {
                // Found
                T& local_value = hash_table[hash]->value;
                mutex.unlock_shared();
                return local_value;
            }
            else
            {
                // Check the next bucket
                hash += step;
                ++step;

                if (hash >= hash_table.size())
                {
                    hash %= hash_table.size();
                }

                ++tries;
                continue;
            }
        }
        else
        {
            // First access

            // We get exclusive lock before calling callback so that the user
            // doesn't have to worry about writing a thread-safe callback.
            mutex.unlock_shared();
            mutex.lock();

            T new_value = createFcn();

            if (hash_table[hash].has_value())
            {
                // Resolve hash collsion by linear probing.
                while (true)
                {
                    hash += step;
                    ++step;

                    if (hash >= hash_table.size())
                    {
                        hash %= hash_table.size();
                    }

                    if (!hash_table[hash].has_value())
                    {
                        break;
                    }
                }
            }

            hash_table[hash].emplace(tid, std::move(new_value));
            T& local_value = hash_table[hash]->value;

            mutex.unlock();

            return local_value;
        }
    }
}

template <typename T>
inline void ThreadLocal<T>::ForEach(std::function<void(std::thread::id tid, T& value)>&& callback)
{
    mutex.lock();
    for (auto& entry : hash_table)
    {
        if (entry.has_value())
        {
            callback(entry->tid, entry->value);
        }
    }
    mutex.unlock();
}

} // namespace bulbit
