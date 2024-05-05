#include "bulbit/integrator.h"
#include "bulbit/light.h"
#include "bulbit/material.h"
#include "bulbit/pdf.h"
#include "bulbit/util.h"

namespace bulbit
{

PathIntegrator::PathIntegrator(const Scene* scene, const Intersectable* accel, const Sampler* sampler, int32 bounces, Float rr)
    : SamplerIntegrator(scene, accel, sampler)
    , max_bounces{ bounces }
    , rr_probability{ rr }
{
}

Spectrum PathIntegrator::Li(const Ray& primary_ray, Sampler& sampler) const
{
    Spectrum L(0), throughput(1);
    bool was_specular_bounce = false;
    Ray ray = primary_ray;

    for (int32 bounce = 0;; ++bounce)
    {
        Intersection is;
        bool found_intersection = Intersect(&is, ray, Ray::epsilon, infinity);
        const Material* mat = is.material;

        if (bounce == 0 || was_specular_bounce)
        {
            if (found_intersection)
            {
                if (mat->IsLightSource())
                {
                    L += throughput * mat->Emit(is, ray.d);
                }
            }
            else
            {
                const std::vector<Light*>& lights = scene->GetInfiniteLights();
                for (Light* light : lights)
                {
                    L += throughput * light->Emit(ray);
                }
            }
        }

        if (found_intersection == false || bounce >= max_bounces)
        {
            break;
        }

        Interaction ir;
        if (mat->Scatter(&ir, is, ray.d, sampler.Next2D()) == false)
        {
            break;
        }

        if ((was_specular_bounce = ir.is_specular))
        {
            throughput *= ir.attenuation;
            ray = ir.specular_ray;
            continue;
        }

        const PDF* spdf = ir.GetScatteringPDF();

        L += throughput * mat->Emit(is, ray.d);

        // Estimate direct light
        // Multiple importance sampling (Direct light + BRDF)

        const std::vector<Light*>& lights = scene->GetLights();
        size_t num_lights = lights.size();
        if (num_lights > 0)
        {
            // Sample one light uniformly
            size_t index = std::min(size_t(sampler.Next1D() * num_lights), num_lights - 1);
            const Light* light = lights[index];
            Float light_weight = Float(num_lights);

            Vec3 to_light;
            Float light_pdf;
            Float visibility;
            Spectrum li = light->Sample(&to_light, &light_pdf, &visibility, is, sampler.Next2D());

            // Importance sample light
            Float light_brdf_pdf = spdf->Evaluate(to_light);
            if (li.IsBlack() == false && light_brdf_pdf > 0)
            {
                Ray shadow_ray(is.point, to_light);
                if (IntersectAny(shadow_ray, Ray::epsilon, visibility) == false)
                {
                    Spectrum f_cos = mat->Evaluate(is, ray.d, to_light);
                    if (light->IsDeltaLight())
                    {
                        L += throughput * light_weight * li * f_cos / light_pdf;
                    }
                    else
                    {
                        Float mis_weight = PowerHeuristic(1, light_pdf, 1, light_brdf_pdf);
                        L += throughput * light_weight * mis_weight * li * f_cos / light_pdf;
                    }
                }
            }

            if (light->IsDeltaLight() == false)
            {
                // Importance sample BRDF
                Vec3 scattered = spdf->Sample(sampler.Next2D());
                Ray shadow_ray(is.point, scattered);

                Float brdf_light_pdf = light->EvaluatePDF(shadow_ray);
                if (brdf_light_pdf > 0)
                {
                    Intersection shadow_is;
                    if (Intersect(&shadow_is, shadow_ray, Ray::epsilon, infinity))
                    {
                        // Intersects area light
                        if (shadow_is.material->IsLightSource())
                        {
                            Float brdf_pdf = spdf->Evaluate(scattered);
                            Float mis_weight = PowerHeuristic(1, brdf_pdf, 1, brdf_light_pdf);

                            li = shadow_is.material->Emit(shadow_is, scattered);
                            if (li.IsBlack() == false)
                            {
                                Spectrum f_cos = mat->Evaluate(is, ray.d, scattered);
                                L += throughput * light_weight * mis_weight * li * f_cos / brdf_pdf;
                            }
                        }
                    }
                    else
                    {
                        li = light->Emit(shadow_ray);
                        if (li.IsBlack() == false)
                        {
                            Float brdf_pdf = spdf->Evaluate(scattered);
                            Float mis_weight = PowerHeuristic(1, brdf_pdf, 1, brdf_light_pdf);

                            Spectrum f_cos = mat->Evaluate(is, ray.d, scattered);
                            L += throughput * light_weight * mis_weight * li * f_cos / brdf_pdf;
                        }
                    }
                }
            }
        }

        // Sample new path direction based on scattering pdf
        Vec3 wi = spdf->Sample(sampler.Next2D());
        Float pdf = spdf->Evaluate(wi);
        if (pdf <= 0)
        {
            break;
        }

        Ray scattered(is.point, wi);

        throughput *= mat->Evaluate(is, ray.d, wi) / pdf;
        ray = scattered;

        // Russian roulette
        const int32 min_bounces = 2;
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

    return L;
}

} // namespace bulbit
