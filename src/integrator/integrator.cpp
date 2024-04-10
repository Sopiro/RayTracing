#include "bulbit/integrator.h"
#include "bulbit/bitmap.h"
#include "bulbit/postprocess.h"

#include "bulbit/parallel_for.h"

namespace bulbit
{

SamplerIntegrator::SamplerIntegrator(const Ref<Sampler> _sampler)
    : sampler_prototype{ _sampler }
{
}

void SamplerIntegrator::Render(Film* film, const Scene& scene, const Camera& camera)
{
    int32 width = film->width;
    int32 height = film->height;

    const int32 tile_size = 16;
    const int32 num_tiles_x = (width + tile_size - 1) / tile_size;
    const int32 num_tiles_y = (height + tile_size - 1) / tile_size;
    const int32 tile_count = num_tiles_x * num_tiles_y;

    int32 p = 0;
    std::thread::id tid = std::this_thread::get_id();
    int32 worker_count = g_thread_pool->WorkerCount();

    ParallelFor(0, tile_count, [&](int32 i) {
        if (tid == std::this_thread::get_id())
        {
            // Log progress if it's calling thread
            std::printf("\rRendering.. %d/%d", p++ * worker_count, tile_count);
        }

        int32 tile_x = i % num_tiles_x;
        int32 tile_y = i / num_tiles_x;

        // Create thread local sampler from prototype for current tile
        std::unique_ptr<Sampler> sampler = sampler_prototype->Clone(i);

        int32 x0 = tile_x * tile_size;
        int32 x1 = std::min(x0 + tile_size, width);
        int32 y0 = tile_y * tile_size;
        int32 y1 = std::min(y0 + tile_size, height);

        for (int32 y = y0; y < y1; ++y)
        {
            for (int32 x = x0; x < x1; ++x)
            {
                sampler->StartPixel();

                Spectrum samples(0);

                do
                {
                    Point2 film_sample = Point2(Float(x), Float(y)) + sampler->Next2D();
                    Point2 aperture_sample = sampler->Next2D();

                    Ray ray;
                    Float weight = camera.SampleRay(&ray, film_sample, aperture_sample);

                    Spectrum L(0);
                    if (weight > 0)
                    {
                        L = Li(scene, ray, *sampler);
                    }

                    if (L.IsNullish())
                    {
                        L = RGBSpectrum::black;
                    }

                    samples += L * weight;
                }
                while (sampler->StartNextPixelSample());

                Spectrum radiance = samples / Float(sampler->samples_per_pixel);

                film->Set(x, y, radiance);
            }
        }
    });

    std::printf("\n");
}

} // namespace bulbit
