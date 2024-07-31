#pragma once

#include "async_job.h"

namespace bulbit
{

class RenderingProgress
{
public:
    RenderingProgress(int32 width, int32 height, int32 tile_size, int32 tile_count)
        : width{ width }
        , height{ height }
        , tile_count{ tile_count }
        , tile_size{ tile_size }
        , tile_done{ 0 }
        , done{ false }
    {
    }

    ~RenderingProgress() = default;

    void Wait()
    {
        job->Wait();
    }

    void WaitAndLogProgress()
    {
        while (!job->Finished())
        {
            int32 t = tile_done.load();
            float p = 100.0f * t / tile_count;
            std::fprintf(stdout, "\rRendering.. %.2f%% [%d/%d]", p, t, tile_count);
            std::this_thread::yield();
        }

        assert(done);
    }

    const int32 width, height;
    const int32 tile_size;
    const int32 tile_count;
    std::atomic<int32> tile_done;
    std::atomic<bool> done;

private:
    friend class UniDirectionalRayIntegrator;

    std::unique_ptr<AsyncJob<bool>> job;
};

} // namespace bulbit
