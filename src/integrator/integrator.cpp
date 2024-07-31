#include "bulbit/bitmap.h"
#include "bulbit/integrators.h"

#include "bulbit/parallel_for.h"

namespace bulbit
{

UniDirectionalRayIntegrator::UniDirectionalRayIntegrator(
    const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler
)
    : Integrator(accel, std::move(lights))
    , sampler_prototype{ sampler }
{
}

void UniDirectionalRayIntegrator::Render(Film* film, const Camera& camera)
{
    int32 width = film->resolution.x;
    int32 height = film->resolution.y;

    const int32 tile_size = 16;
    int32 num_tiles_x = (width + tile_size - 1) / tile_size;
    int32 num_tiles_y = (height + tile_size - 1) / tile_size;
    int32 tile_count = num_tiles_x * num_tiles_y;

    const int32 spp = sampler_prototype->samples_per_pixel;
    std::atomic<int32> tile_done = 0;

    ParallelFor(0, tile_count, [&](int32 i) {
        std::printf("\rRendering.. %d/%d", tile_done++, tile_count);

        Point2i tile(i % num_tiles_x, i / num_tiles_x);

        // Create thread local sampler for current tile
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
                        film->AddSample(x, y, L, 1);
                    }
                }
            }
        }
    });

    std::printf("\n");
}

} // namespace bulbit
