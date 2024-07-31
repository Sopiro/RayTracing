#include "bulbit/bitmap.h"
#include "bulbit/integrators.h"

#include "bulbit/async_job.h"
#include "bulbit/parallel_for.h"
#include "bulbit/progress.h"

namespace bulbit
{

UniDirectionalRayIntegrator::UniDirectionalRayIntegrator(
    const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler
)
    : Integrator(accel, std::move(lights))
    , sampler_prototype{ sampler }
{
}

std::unique_ptr<RenderingProgress> UniDirectionalRayIntegrator::Render(const Camera& camera)
{
    Point2i resolution = camera.GetScreenResolution();
    int32 width = resolution.x;
    int32 height = resolution.y;

    const int32 spp = sampler_prototype->samples_per_pixel;
    const int32 tile_size = 16;
    int32 num_tiles_x = (width + tile_size - 1) / tile_size;
    int32 num_tiles_y = (height + tile_size - 1) / tile_size;
    int32 tile_count = num_tiles_x * num_tiles_y;

    std::unique_ptr<RenderingProgress> progress = std::make_unique<RenderingProgress>(resolution, tile_size, tile_count);

    progress->job = RunAsync([=, this, &progress, &camera]() {
        std::atomic<int32> tile_done = 0;

        ParallelFor(0, tile_count, [&](int32 i) {
            Point2i tile(i % num_tiles_x, i / num_tiles_x);

            // Thread local sampler for current tile
            std::unique_ptr<Sampler> sampler = sampler_prototype->Clone();

            int32 x0 = tile.x * tile_size;
            int32 x1 = std::min(x0 + tile_size, width);
            int32 y0 = tile.y * tile_size;
            int32 y1 = std::min(y0 + tile_size, height);

            for (int32 y = y0; y < y1; ++y)
            {
                for (int32 x = x0; x < x1; ++x)
                {
                    Point2i pixel = Point2i(x, y);

                    for (int32 sample = 0; sample < spp; ++sample)
                    {
                        sampler->StartPixelSample(pixel, sample);

                        Ray ray;
                        Float weight = camera.SampleRay(&ray, pixel, sampler->Next2D(), sampler->Next2D());

                        Spectrum L = weight * Li(ray, camera.GetMedium(), *sampler);

                        if (!L.IsNullish())
                        {
                            progress->film.AddSample(x, y, L, 1);
                        }
                    }
                }
            }

            progress->tile_done++;
        });

        progress->done = true;
        return true;
    });

    return progress;
}

} // namespace bulbit
