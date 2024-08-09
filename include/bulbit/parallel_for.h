#pragma once

#include "bounding_box.h"
#include "parallel.h"

namespace bulbit
{

class ParallelForLoop : public ParallelJob
{
public:
    ParallelForLoop(int32 begin_index, int32 end_index, int32 chunk_size, std::function<void(int32, int32)> func)
        : next_index{ begin_index }
        , end_index{ end_index }
        , chunk_size{ chunk_size }
        , func{ std::move(func) }
    {
        assert(begin_index < end_index);
    }

    virtual bool HaveWork() const override
    {
        return next_index < end_index;
    }

    virtual void RunStep(std::unique_lock<std::mutex>* lock) override;

private:
    std::function<void(int32, int32)> func;
    int32 next_index;
    const int32 end_index;
    int32 chunk_size;
};

void ParallelFor(
    int32 begin,
    int32 end,
    std::function<void(int32 begin, int32 end)> func,
    ThreadPool* thread_pool = ThreadPool::global_thread_pool.get()
);

inline void ParallelFor(
    int32 begin, int32 end, std::function<void(int32 i)> func, ThreadPool* thread_pool = ThreadPool::global_thread_pool.get()
)
{
    ParallelFor(
        begin, end,
        [&func](int32 begin, int32 end) {
            for (int32 i = begin; i < end; ++i)
            {
                func(i);
            }
        },
        thread_pool
    );
}

inline void ParallelFor2D(
    const Point2i& extents,
    std::function<void(AABB2i tile)> func,
    int32 tile_size = 16,
    ThreadPool* thread_pool = ThreadPool::global_thread_pool.get()
)
{
    int32 num_tiles_x = (extents.x + tile_size - 1) / tile_size;
    int32 num_tiles_y = (extents.y + tile_size - 1) / tile_size;
    int32 tile_count = num_tiles_x * num_tiles_y;

    ParallelFor(
        0, tile_count,
        [&](int32 i) {
            Point2i tile(i % num_tiles_x, i / num_tiles_x);

            int32 x0 = tile.x * tile_size;
            int32 x1 = std::min(x0 + tile_size, extents.x);
            int32 y0 = tile.y * tile_size;
            int32 y1 = std::min(y0 + tile_size, extents.y);

            func(AABB2i(Point2i(x0, y0), Point2i(x1, y1)));
        },
        thread_pool
    );
}

} // namespace bulbit
