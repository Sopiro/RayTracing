#include "bulbit/path_integrator.h"
#include "bulbit/brdf.h"
#include "bulbit/infinite_area_light.h"
#include "bulbit/light.h"
#include "bulbit/scene.h"
#include "bulbit/util.h"

namespace bulbit
{

PathIntegrator::PathIntegrator(const Ref<Sampler> sampler, int32 bounces, Float rr)
    : SamplerIntegrator(sampler)
    , max_bounces{ bounces }
    , rr_probability{ rr }
{
}

Spectrum PathIntegrator::Li(const Scene& scene, const Ray& primary_ray, Sampler& sampler) const
{
    Spectrum radiance(0), throughput(1);
    bool was_specular_bounce = false;
    Ray ray = primary_ray;

    for (int32 bounce = 0;; ++bounce)
    {
        Intersection is;
        const Material* mat;
        bool found_intersection = scene.Intersect(&is, ray, Ray::epsilon, infinity);

        if (bounce == 0 || was_specular_bounce)
        {
            if (found_intersection)
            {
                mat = scene.GetMaterial(is.material_index);
                if (mat->IsLightSource())
                {
                    radiance += throughput * mat->Emit(is, ray.d);
                }
            }
            else
            {
                const std::vector<InfiniteAreaLight*>& lights = scene.GetInfiniteAreaLights();
                for (InfiniteAreaLight* light : lights)
                {
                    radiance += throughput * light->Emit(ray);
                }
            }
        }

        if (found_intersection == false || bounce >= max_bounces)
        {
            break;
        }

        mat = scene.GetMaterial(is.material_index);

        Interaction ir;
        if (mat->Scatter(&ir, is, ray.d, sampler.Next2D()) == false)
        {
            break;
        }

        if (was_specular_bounce = ir.is_specular)
        {
            throughput *= ir.attenuation;
            ray = ir.specular_ray;
            continue;
        }

        const BRDF* brdf = ir.GetScatteringPDF();

        radiance += throughput * mat->Emit(is, ray.d);

        // Estimate direct light
        // Multiple importance sampling (Direct light + BRDF)

        const std::vector<Ref<Light>>& lights = scene.GetLights();
        size_t num_lights = lights.size();
        if (num_lights > 0)
        {
            // Sample one light uniformly
            size_t index = std::min(size_t(sampler.Next1D() * num_lights), num_lights - 1);
            Light* light = lights[index].get();
            Float light_weight = Float(num_lights);

            Vec3 to_light;
            Float light_pdf;
            Float visibility;
            Spectrum li = light->Sample(&to_light, &light_pdf, &visibility, is, sampler.Next2D());

            Ray shadow_ray{ is.point, to_light };

            // Importance sample light
            Float light_brdf_pdf = brdf->Evaluate(to_light);
            if (li.IsBlack() == false && light_brdf_pdf > 0)
            {
                if (scene.IntersectAny(shadow_ray, Ray::epsilon, visibility) == false)
                {
                    Spectrum f_cos = mat->Evaluate(is, ray.d, to_light);
                    if (light->IsDeltaLight())
                    {
                        radiance += throughput * light_weight / light_pdf * li * f_cos;
                    }
                    else
                    {
                        Float mis_weight = PowerHeuristic(1, light_pdf, 1, light_brdf_pdf);
                        radiance += throughput * light_weight * mis_weight / light_pdf * li * f_cos;
                    }
                }
            }

            if (light->IsDeltaLight() == false)
            {
                // Importance sample BRDF
                Vec3 scattered = brdf->Sample(sampler.Next2D());
                shadow_ray = Ray{ is.point, scattered };

                Float brdf_light_pdf = light->EvaluatePDF(shadow_ray);
                if (brdf_light_pdf > 0)
                {
                    Intersection shadow_is;
                    if (scene.Intersect(&shadow_is, shadow_ray, Ray::epsilon, infinity))
                    {
                        const Material* shadow_mat = scene.GetMaterial(shadow_is.material_index);

                        // Intersects area light
                        if (shadow_mat->IsLightSource())
                        {
                            Float brdf_pdf = brdf->Evaluate(scattered);
                            Float mis_weight = PowerHeuristic(1, brdf_pdf, 1, brdf_light_pdf);

                            li = shadow_mat->Emit(shadow_is, scattered);
                            if (li.IsBlack() == false)
                            {
                                Spectrum f_cos = mat->Evaluate(is, ray.d, scattered);
                                radiance += throughput * light_weight * mis_weight / brdf_pdf * li * f_cos;
                            }
                        }
                    }
                    else
                    {
                        li = light->Emit(shadow_ray);
                        if (li.IsBlack() == false)
                        {
                            Float brdf_pdf = brdf->Evaluate(scattered);
                            Float mis_weight = PowerHeuristic(1, brdf_pdf, 1, brdf_light_pdf);

                            Spectrum f_cos = mat->Evaluate(is, ray.d, scattered);
                            radiance += throughput * light_weight * mis_weight / brdf_pdf * li * f_cos;
                        }
                    }
                }
            }
        }

        // Sample new path direction based on BRDF
        Vec3 wi = brdf->Sample(sampler.Next2D());
        Float pdf = brdf->Evaluate(wi);
        if (pdf <= 0)
        {
            break;
        }

        Ray scattered(is.point, wi);

        throughput *= mat->Evaluate(is, ray.d, wi) / pdf;
        ray = scattered;

        // Russian roulette
        const int32 min_bounces = 3;
        if (bounce > min_bounces)
        {
            Float rr = std::fmin(rr_probability, throughput.Luminance());
            if (sampler.Next1D() > rr)
            {
                break;
            }

            throughput *= 1 / rr;
        }
    }

    return radiance;
}

} // namespace bulbit
