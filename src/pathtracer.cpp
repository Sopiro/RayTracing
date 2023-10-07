#include "spt/spt.h"
#include "spt/util.h"

#define MIN_BOUNCES 3

namespace spt
{

Color PathTrace(const Scene& scene, Ray ray, int32 max_bounces)
{
    Color radiance(0, 0, 0);
    Vec3 throughput(1, 1, 1);

    bool was_specular = false;

    for (int32 bounce = 0;; ++bounce)
    {
        Intersection is;
        if (scene.Intersect(&is, ray, Ray::epsilon, infinity) == false)
        {
            const std::vector<InfiniteAreaLight*>& lights = scene.GetInfiniteAreaLights();
            for (InfiniteAreaLight* light : lights)
            {
                radiance += throughput * light->Emit(ray);
            }
            break;
        }

        const Material* mat = is.material;

        Interaction ir;
        if (mat->Scatter(&ir, is, ray.d) == false)
        {
            if (bounce == 0 || was_specular == true)
            {
                radiance += throughput * mat->Emit(is, ray.d);
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

        const PDF* pdf = ir.GetScatteringPDF();

        radiance += throughput * mat->Emit(is, ray.d);

        // Estimate direct light
        // Multiple importance sampling with balance heuristic (Direct light + BRDF)

        const std::vector<Ref<Light>>& lights = scene.GetLights();
        size_t num_lights = lights.size();
        if (num_lights > 0)
        {
            // Sample one light uniformly
            size_t index = std::min(size_t(Rand() * num_lights), num_lights - 1);
            Light* light = lights[index].get();

            Vec3 to_light;
            Float light_pdf;
            Float visibility;
            Color li = light->Sample(&to_light, &light_pdf, &visibility, is);

            Ray shadow_ray{ is.point, to_light };

            // Importance sample light
            Float light_brdf_pdf = pdf->Evaluate(to_light);
            if (IsBlack(li) == false && light_brdf_pdf > 0)
            {
                if (scene.IntersectAny(shadow_ray, Ray::epsilon, visibility) == false)
                {
                    if (light->IsDeltaLight())
                    {
                        radiance += throughput * Float(num_lights) / light_pdf * li * mat->Evaluate(is, ray.d, to_light);
                    }
                    else
                    {
                        Float mis_weight = 1 / (light_pdf + light_brdf_pdf);
                        radiance += throughput * Float(num_lights) * mis_weight * li * mat->Evaluate(is, ray.d, to_light);
                    }
                }
            }

            if (light->IsDeltaLight() == false)
            {
                // Importance sample BRDF
                Vec3 scattered = pdf->Sample();
                shadow_ray = Ray{ is.point, scattered };

                Float brdf_light_pdf = light->EvaluatePDF(shadow_ray);
                if (brdf_light_pdf > 0)
                {
                    Intersection is2;
                    if (scene.Intersect(&is2, shadow_ray, Ray::epsilon, infinity))
                    {
                        if (is2.object == ((AreaLight*)light)->GetPrimitive())
                        {
                            Float brdf_pdf = pdf->Evaluate(scattered);
                            Float mis_weight = 1 / (brdf_pdf + brdf_light_pdf);

                            li = is2.material->Emit(is2, scattered);
                            if (IsBlack(li) == false)
                            {
                                radiance +=
                                    throughput * Float(num_lights) * mis_weight * li * mat->Evaluate(is, ray.d, scattered);
                            }
                        }
                    }
                    else
                    {
                        li = light->Emit(shadow_ray);
                        if (IsBlack(li) == false)
                        {
                            Float brdf_pdf = pdf->Evaluate(scattered);
                            Float mis_weight = 1 / (brdf_pdf + brdf_light_pdf);
                            radiance += throughput * Float(num_lights) * mis_weight * li * mat->Evaluate(is, ray.d, scattered);
                        }
                    }
                }
            }
        }

        // Sample new search direction based on BRDF
        Vec3 wi = pdf->Sample();
        Float pdf_value;

        if (Dot(is.normal, wi) > 0)
        {
            pdf_value = pdf->Evaluate(wi);
        }
        else
        {
            break;
        }

        assert(pdf_value > 0);
        Ray scattered{ is.point, wi };

        throughput *= mat->Evaluate(is, ray.d, wi) / pdf_value;
        ray = scattered;

        // Russian roulette
        if (bounce > MIN_BOUNCES)
        {
            Float rr = std::fmin(Float(0.95), Luma(throughput));
            if (Rand() > rr)
            {
                break;
            }

            throughput *= 1 / rr;
        }
    }

    return radiance;
}

} // namespace spt
