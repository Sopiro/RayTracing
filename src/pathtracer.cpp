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
        Vec3 d = Normalize(ray.d);

        Intersection is;
        if (scene.Intersect(&is, ray, ray_epsilon, infinity) == false)
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

        if (scene.HasLights())
        {
            // Multiple importance sampling with balance heuristic (Direct light + BRDF)

            // Sample one area light uniformly
            const std::vector<Ref<Light>>& lights = scene.GetLights();
            size_t num_lights = lights.size();
            size_t index = std::min(size_t(Rand() * num_lights), num_lights - 1);
            Light* light = lights[index].get();

            Vec3 to_light;
            f64 light_pdf;
            f64 visibility;
            Color li = light->Sample(&to_light, &light_pdf, &visibility, is);

            Ray shadow_ray{ is.point, to_light };

            // Importance sample light
            f64 light_brdf_pdf = ir.pdf->Evaluate(to_light);
            if (IsBlack(li) == false && light_brdf_pdf > 0.0)
            {
                if (scene.IntersectAny(shadow_ray, ray_epsilon, visibility - ray_epsilon) == false)
                {
                    f64 mis_weight = 1.0 / (light_pdf + light_brdf_pdf);

                    radiance += throughput * f64(num_lights) * mis_weight * li * mat->Evaluate(is, d, to_light);
                }
            }

            if (light->IsDeltaLight() == false)
            {
                // Importance sample BRDF
                Vec3 scattered = ir.pdf->Sample();
                shadow_ray = Ray{ is.point, scattered };

                f64 brdf_light_pdf = light->EvaluatePDF(shadow_ray);
                if (brdf_light_pdf > 0.0)
                {
                    Intersection is2;
                    if (scene.Intersect(&is2, shadow_ray, ray_epsilon, infinity))
                    {
                        if (is2.object == ((AreaLight*)light)->GetPrimitive())
                        {
                            f64 brdf_p = ir.pdf->Evaluate(scattered);
                            f64 mis_weight = 1.0 / (brdf_p + brdf_light_pdf);

                            li = is2.material->Emit(is2, scattered);
                            if (IsBlack(li) == false)
                            {
                                radiance += throughput * f64(num_lights) * mis_weight * li * mat->Evaluate(is, d, scattered);
                            }
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
