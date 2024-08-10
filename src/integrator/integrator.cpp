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

    const int32 spp = sampler_prototype->samples_per_pixel;
    const int32 tile_size = 16;

    std::unique_ptr<RenderingProgress> progress = std::make_unique<RenderingProgress>(resolution, tile_size);

    progress->job = RunAsync([=, this, &progress, &camera]() {
        ParallelFor2D(
            resolution,
            [&](AABB2i tile) {
                // Thread local sampler for current tile
                std::unique_ptr<Sampler> sampler = sampler_prototype->Clone();

                for (Point2i pixel : tile)
                {
                    for (int32 sample = 0; sample < spp; ++sample)
                    {
                        sampler->StartPixelSample(pixel, sample);

                        Ray ray;
                        Float weight = camera.SampleRay(&ray, pixel, sampler->Next2D(), sampler->Next2D());

                        Spectrum L = Li(ray, camera.GetMedium(), *sampler);

                        if (!L.IsNullish())
                        {
                            progress->film.AddSample(pixel, weight * L, 1);
                        }
                    }
                }

                progress->tile_done++;
            },
            tile_size
        );

        progress->done = true;
        return true;
    });

    return progress;
}

} // namespace bulbit
