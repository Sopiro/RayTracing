#include "spt/pathtracer.h"

#define MIN_BOUNCES 3

namespace spt
{

Color PathTrace(const Scene& scene, Ray ray, i32 max_bounces)
{
    Color radiance{ 0.0 };
    Vec3 throughput{ 1.0 };

    bool was_specular = false;

    for (i32 bounce = 0;; ++bounce)
    {
        Intersection is;
        if (scene.Intersect(&is, ray, ray_offset, infinity) == false)
        {
            radiance += throughput * scene.GetSkyColor(ray.dir);
            break;
        }

        const Material* mat = is.object->GetMaterial();

        Interaction ir;
        if (mat->Scatter(&ir, is, ray) == false)
        {
            if (bounce == 0 || was_specular == true)
            {
                radiance += throughput * mat->Emit(is, ray);
            }
            break;
        }

        if (bounce >= max_bounces)
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

        radiance += throughput * mat->Emit(is, ray);

        // Evaluate direct light (Next Event Estimation)

        if (scene.HasDirectionalLight())
        {
            const Ref<DirectionalLight>& sun = scene.GetDirectionalLight();
            Ray to_sun{ is.point + is.normal * ray_offset, -sun->dir + RandomInUnitSphere() * sun->radius };

            Intersection is2;
            if (scene.Intersect(&is2, to_sun, ray_offset, infinity) == false)
            {
                radiance += throughput * sun->radiance * mat->Evaluate(is, ray, to_sun);
            }
        }

        if (scene.HasAreaLights())
        {
            // Multiple importance sampling with balance heuristic (Direct light + BRDF)

            // Sample one area light uniformly
            IntersectablePDF light_pdf{ &scene.GetAreaLights(), is.point };

            // Importance sample lights
            Ray to_light{ is.point, light_pdf.Sample() };
            if (Dot(to_light.dir, is.normal) > 0.0)
            {
                f64 light_brdf_p = ir.pdf->Evaluate(to_light.dir);
                if (light_brdf_p > 0.0)
                {
                    f64 light_p = light_pdf.Evaluate(to_light.dir);
                    f64 mis_w = 1.0 / (light_p + light_brdf_p);

                    Intersection is2;
                    if (scene.Intersect(&is2, to_light, ray_offset, infinity))
                    {
                        Color li = is2.object->GetMaterial()->Emit(is2, to_light);
                        radiance += throughput * mis_w * li * mat->Evaluate(is, ray, to_light);
                    }
                    else
                    {
                        Color li = scene.GetSkyColor(to_light.dir);
                        radiance += throughput * mis_w * li * mat->Evaluate(is, ray, to_light);
                    }
                }
            }

            // Importance sample BRDF
            Ray scattered{ is.point, ir.pdf->Sample() };
            if (Dot(scattered.dir, is.normal) > 0.0)
            {
                f64 brdf_light_p = light_pdf.Evaluate(scattered.dir);
                if (brdf_light_p > 0.0)
                {
                    f64 brdf_p = ir.pdf->Evaluate(scattered.dir);
                    f64 mis_w = 1.0 / (brdf_p + brdf_light_p);

                    Intersection is2;
                    if (scene.Intersect(&is2, scattered, ray_offset, infinity))
                    {
                        Color li = is2.object->GetMaterial()->Emit(is2, scattered);
                        radiance += throughput * mis_w * li * mat->Evaluate(is, ray, scattered);
                    }
                    else
                    {
                        Color li = scene.GetSkyColor(scattered.dir);
                        radiance += throughput * mis_w * li * mat->Evaluate(is, ray, scattered);
                    }
                }
            }
        }

        // Sample new search direction based on BRDF
        Vec3 wi = ir.pdf->Sample();
        f64 pdf;

        if (Dot(is.normal, wi) > 0.0)
        {
            pdf = ir.pdf->Evaluate(wi);
        }
        else
        {
            break;
        }

        assert(pdf > 0.0);
        Ray scattered{ is.point, wi };

        throughput *= mat->Evaluate(is, ray, scattered) / pdf;
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
