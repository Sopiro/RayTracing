#include "bulbit/integrator.h"
#include "bulbit/bitmap.h"
#include "bulbit/postprocess.h"

#include <omp.h>

namespace bulbit
{

SamplerIntegrator::SamplerIntegrator(const Ref<Sampler> _sampler)
    : sampler{ _sampler }
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
        int32 tile_x = i % num_tiles_x;
        int32 tile_y = i / num_tiles_x;

        std::unique_ptr<Sampler> tile_sampler = sampler->Clone(i);

        int32 x0 = tile_x * tile_size;
        int32 x1 = std::min(x0 + tile_size, width);
        int32 y0 = tile_y * tile_size;
        int32 y1 = std::min(y0 + tile_size, height);

        for (int32 y = y0; y < y1; ++y)
        {
            for (int32 x = x0; x < x1; ++x)
            {
                tile_sampler->StartPixel();

                Spectrum samples(0);

                do
                {
                    Point2 film_sample((x + sampler->Next1D()) / width, (y + sampler->Next1D()) / height);
                    Point2 aperture_sample = sampler->Next2D();

                    Ray ray;
                    Float weight = camera.SampleRay(&ray, film_sample, aperture_sample);

                    Spectrum l(0);
                    if (weight > 0)
                    {
                        l = Li(scene, ray, *tile_sampler);
                    }

                    if (l.IsNullish())
                    {
                        l = RGBSpectrum::black;
                    }

                    samples += l * weight;
                }
                while (tile_sampler->StartNextPixelSample());

                Spectrum radiance = samples / sampler->samples_per_pixel;

                film->Set(x, y, radiance);
            }
        }
    }
}

} // namespace bulbit
