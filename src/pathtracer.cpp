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
        Vec3 d = ray.d.Normalized();

        Intersection is;
        if (scene.Intersect(&is, ray, ray_offset, infinity) == false)
        {
            radiance += throughput * scene.GetSkyColor(d);
            break;
        }

        const Material* mat = is.material;

        Interaction ir;
        if (mat->Scatter(&ir, is, d) == false)
        {
            if (bounce == 0 || was_specular == true)
            {
                radiance += throughput * mat->Emit(is, d);
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

        radiance += throughput * mat->Emit(is, d);

        // Estimate direct light

        if (scene.HasDirectionalLight())
        {
            const Ref<DirectionalLight> sun = scene.GetDirectionalLight();
            Ray to_sun{ is.point + is.normal * ray_offset, -sun->dir + RandomInUnitSphere() * sun->radius };

            if (scene.IntersectAny(to_sun, ray_offset, infinity) == false)
            {
                radiance += throughput * sun->radiance * mat->Evaluate(is, d, to_sun.d.Normalized());
            }
        }

        if (scene.HasAreaLights())
        {
            // Multiple importance sampling with balance heuristic (Direct light + BRDF)

            // Sample one area light uniformly
            const std::vector<Ref<Primitive>>& lights = scene.GetAreaLights();
            size_t num_lights = lights.size();
            size_t index = std::min(size_t(Rand() * num_lights), num_lights - 1);
            Ref<Primitive> light = lights[index];

            Vec3 l;
            SurfaceSample light_sample;
            light->Sample(&light_sample, &l, is.point);

            f64 len = l.Normalize();
            Ray to_light{ is.point, l };

            // Importance sample lights
            if (Dot(l, is.normal) > 0.0)
            {
                f64 light_brdf_p = ir.pdf->Evaluate(l);
                if (light_brdf_p > 0.0)
                {
                    if (scene.IntersectAny(to_light, ray_offset, len - ray_offset) == false)
                    {
                        f64 mis_w = 1.0 / (light_sample.pdf + light_brdf_p);

                        Intersection is2;
                        is2.front_face = Dot(light_sample.n, l) < 0.0;
                        Color li = light->GetMaterial()->Emit(is2, l);
                        radiance += throughput * f64(num_lights) * mis_w * li * mat->Evaluate(is, d, l);
                    }
                }
            }

            // Importance sample BRDF
            Vec3 s = ir.pdf->Sample();

            Ray scattered{ is.point, s };
            if (Dot(s, is.normal) > 0.0)
            {
                f64 brdf_light_p = light->EvaluatePDF(scattered);
                if (brdf_light_p > 0.0)
                {
                    Intersection is2;
                    if (scene.Intersect(&is2, scattered, ray_offset, infinity))
                    {
                        if (is2.object == light.get())
                        {
                            f64 brdf_p = ir.pdf->Evaluate(s);
                            f64 mis_w = 1.0 / (brdf_p + brdf_light_p);

                            Color li = light->GetMaterial()->Emit(is2, s);
                            radiance += throughput * f64(num_lights) * mis_w * li * mat->Evaluate(is, d, s);
                        }
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

        throughput *= mat->Evaluate(is, d, wi) / pdf;
        ray = scattered;

        // Russian roulette
        if (bounce > MIN_BOUNCES)
        {
            f64 rr = std::fmin(0.95, Luma(throughput));
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
