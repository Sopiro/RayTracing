#include "spt/pathtracer.h"

#define MIN_BOUNCES 3

namespace spt
{

Color PathTrace(const Scene& scene, Ray ray, i32 bounce_count)
{
    Color radiance{ 0.0 };
    Vec3 throughput{ 1.0 };

    bool was_specular = false;

    for (i32 bounce = 0;; ++bounce)
    {
        Intersection is;
        if (scene.Intersect(ray, ray_offset, infinity, is) == false)
        {
            radiance += throughput * scene.GetSkyColor(ray.dir);
            break;
        }

        Interaction ir;
        if (is.mat->Scatter(ray, is, ir) == false)
        {
            if (bounce == 0 || was_specular == true)
            {
                radiance += throughput * is.mat->Emit(ray, is);
            }
            break;
        }

        if (bounce >= bounce_count)
        {
            break;
        }

        if (ir.is_specular == true)
        {
            throughput *= ir.attenuation;
            ray = ir.specular_ray;
            was_specular = true;
            continue;
        }
        else
        {
            was_specular = false;
        }

        radiance += throughput * is.mat->Emit(ray, is);

        // Evaluate direct light (Next Event Estimation)

        if (scene.HasDirectionalLight())
        {
            const Ref<DirectionalLight>& sun = scene.GetDirectionalLight();
            Ray to_sun{ is.point + is.normal * ray_offset, -sun->dir };

            Intersection rec2;
            if (scene.Intersect(to_sun, ray_offset, infinity, rec2) == false)
            {
                radiance += throughput * sun->radiance * is.mat->Evaluate(ray, is, to_sun);
            }
        }

        if (scene.HasAreaLights())
        {
            // Multiple importance sampling with balance heuristic (Direct light + BRDF)

            // Sample one light uniformly
            IntersectablePDF light_pdf{ &scene.GetAreaLights(), is.point };

            // Importance sample lights
            Ray to_light{ is.point, light_pdf.Generate() };
            if (Dot(to_light.dir, is.normal) > 0.0)
            {
                f64 light_brdf_p = ir.pdf->Evaluate(to_light.dir);
                if (light_brdf_p > 0.0)
                {
                    Intersection rec2;
                    if (scene.Intersect(to_light, ray_offset, infinity, rec2))
                    {
                        f64 light_p = light_pdf.Evaluate(to_light.dir);
                        f64 mis_w = 1.0 / (light_p + light_brdf_p);

                        radiance += throughput * mis_w * rec2.mat->Emit(to_light, rec2) * is.mat->Evaluate(ray, is, to_light);
                    }
                }
            }

            // Importance sample BRDF
            Ray scattered{ is.point, ir.pdf->Generate() };
            if (Dot(scattered.dir, is.normal) > 0.0)
            {
                f64 brdf_light_p = light_pdf.Evaluate(scattered.dir);
                if (brdf_light_p > 0.0)
                {
                    Intersection rec2;
                    if (scene.Intersect(scattered, ray_offset, infinity, rec2))
                    {
                        f64 brdf_p = ir.pdf->Evaluate(scattered.dir);
                        f64 mis_w = 1.0 / (brdf_p + brdf_light_p);

                        radiance += throughput * mis_w * rec2.mat->Emit(scattered, rec2) * is.mat->Evaluate(ray, is, scattered);
                    }
                }
            }
        }

        // Sample new search direction based on BRDF
        Vec3 new_direction = ir.pdf->Generate();
        f64 pdf_value;

        if (Dot(is.normal, new_direction) > 0.0)
        {
            pdf_value = ir.pdf->Evaluate(new_direction);
        }
        else
        {
            break;
        }

        assert(pdf_value > 0.0);
        Ray scattered{ is.point, new_direction };

        throughput *= is.mat->Evaluate(ray, is, scattered) / pdf_value;
        ray = scattered;

        // Russian roulette
        if (bounce > MIN_BOUNCES)
        {
            f64 rr = fmin(0.95, Luma(throughput));
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
