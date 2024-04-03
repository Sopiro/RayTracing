#include "bulbit/integrator.h"
#include "bulbit/bitmap.h"
#include "bulbit/postprocess.h"

#include <omp.h>

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

#pragma omp parallel for schedule(dynamic, 1)
    for (int32 i = 0; i < num_tiles_x * num_tiles_y; ++i)
    {
        std::printf("\rRendering tile %d/%d", i, num_tiles_x * num_tiles_y);

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

                    Spectrum l(0);
                    if (weight > 0)
                    {
                        l = Li(scene, ray, *sampler);
                    }

                    if (l.IsNullish())
                    {
                        l = RGBSpectrum::black;
                    }

                    samples += l * weight;
                }
                while (sampler->StartNextPixelSample());

                Spectrum radiance = samples / Float(sampler->samples_per_pixel);

                film->Set(x, y, radiance);
            }
        }
    }

    std::printf("\n");
}

} // namespace bulbit
