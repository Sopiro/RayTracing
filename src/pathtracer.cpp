#pragma once

#include "spt/pathtracer.h"

#define MIN_BOUNCES 3
#define MAX_RESAMPLE 10

namespace spt
{

Color PathTrace(const Scene& scene, Ray ray, int32 bounce_count)
{
    Color radiance{ 0.0 };
    Vec3 throughput{ 1.0 };

    bool was_specular = false;

    for (int32 bounce = 0; bounce < bounce_count; ++bounce)
    {
        HitRecord rec;
        if (scene.Hit(ray, ray_offset, infinity, rec) == false)
        {
            radiance += throughput * scene.GetSkyColor(ray.dir);
            break;
        }

        Color emitted = rec.mat->Emit(ray, rec);

        ScatterRecord srec;
        if (rec.mat->Scatter(ray, rec, srec) == false)
        {
            if (bounce == 0 || was_specular == true)
            {
                radiance += throughput * emitted;
            }
            break;
        }

        if (srec.is_specular == true)
        {
            throughput *= srec.attenuation;
            ray = srec.specular_ray;
            was_specular = true;
            continue;
        }
        else
        {
            was_specular = false;
        }

        // Evaluate direct light (next event estimation)

        if (scene.HasDirectionalLight())
        {
            const Ref<DirectionalLight>& sun = scene.GetDirectionalLight();
            Ray to_sun{ rec.point + rec.normal * ray_offset, -sun->dir };

            HitRecord rec2;
            if (scene.Hit(to_sun, ray_offset, infinity, rec2) == false)
            {
                radiance += throughput * sun->radiance * rec.mat->Evaluate(ray, rec, to_sun);
            }
        }

        if (scene.HasAreaLights())
        {
            // Multiple importance sampling with balance heuristic (Direct light + BRDF)

            // Sample one light uniformly
            HittablePDF light_pdf{ &scene.GetAreaLights(), rec.point };

            // Importance sample lights
            Ray to_light{ rec.point, light_pdf.Generate() };
            if (Dot(to_light.dir, rec.normal) > 0.0)
            {
                float64 light_brdf_p = srec.pdf->Evaluate(to_light.dir);
                if (light_brdf_p > 0.0)
                {
                    HitRecord rec2;
                    if (scene.Hit(to_light, ray_offset, infinity, rec2))
                    {
                        float64 light_p = light_pdf.Evaluate(to_light.dir);
                        float64 w = 1.0 / (light_p + light_brdf_p);
                        radiance += w * throughput * rec2.mat->Emit(to_light, rec2) * rec.mat->Evaluate(ray, rec, to_light);
                    }
                }
            }

            // Importance sample BRDF
            Ray scattered{ rec.point, srec.pdf->Generate() };
            if (Dot(scattered.dir, rec.normal) > 0.0)
            {
                float64 brdf_light_p = light_pdf.Evaluate(scattered.dir);
                if (brdf_light_p > 0.0)
                {
                    HitRecord rec2;
                    if (scene.Hit(scattered, ray_offset, infinity, rec2))
                    {
                        float64 brdf_p = srec.pdf->Evaluate(scattered.dir);
                        float64 w = 1.0 / (brdf_p + brdf_light_p);
                        radiance += w * throughput * rec2.mat->Emit(scattered, rec2) * rec.mat->Evaluate(ray, rec, scattered);
                    }
                }
            }
        }

        // Sample new search direction based on BRDF
#if 1
        Vec3 new_direction = srec.pdf->Generate();
        float64 pdf_value;

        if (Dot(rec.normal, new_direction) > 0.0)
        {
            pdf_value = srec.pdf->Evaluate(new_direction);
        }
        else
        {
            break;
        }
#else
        int32 count = 0;
        Vec3 new_direction;
        do
        {
            new_direction = srec.pdf->Generate();
        }
        while (Dot(rec.normal, new_direction) <= 0.0 && count++ < MAX_RESAMPLE);

        if (count >= MAX_RESAMPLE)
        {
            break;
        }

        float64 pdf_value = srec.pdf->Evaluate(new_direction);
#endif

        assert(pdf_value > 0.0);
        Ray scattered{ rec.point, new_direction };

        radiance += throughput * emitted;
        throughput *= rec.mat->Evaluate(ray, rec, scattered) / pdf_value;
        ray = scattered;

        // Russian roulette
        if (bounce > MIN_BOUNCES)
        {
            float64 rr = fmin(0.95, Luma(throughput));
            if (Rand() > rr)
            {
                break;
            }

            throughput *= 1.0 / rr;
        }
    }

    return radiance;
}

} // namespace spt
