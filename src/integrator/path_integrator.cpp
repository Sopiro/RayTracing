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
    , light_sampler{ scene->GetLights() }
{
    for (Light* light : scene->GetLights())
    {
        if (light->type == Light::Type::infinite_light)
        {
            infinite_lights.push_back(light);
        }
    }
}

Spectrum PathIntegrator::Li(const Ray& primary_ray, Sampler& sampler) const
{
    Spectrum L(0), throughput(1);
    bool was_specular_bounce = false;
    Ray ray = primary_ray;

    for (int32 bounce = 0;; ++bounce)
    {
        Intersection isect;
        bool found_intersection = Intersect(&isect, ray, Ray::epsilon, infinity);
        const Material* mat = isect.material;

        if (bounce == 0 || was_specular_bounce)
        {
            if (found_intersection)
            {
                if (mat->IsLightSource())
                {
                    L += throughput * mat->Emit(isect, ray.d);
                }
            }
            else
            {
                for (Light* light : infinite_lights)
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
        if (mat->Scatter(&ir, isect, ray.d, sampler.Next2D()) == false)
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

        L += throughput * mat->Emit(isect, ray.d);

        // Estimate direct light
        // Multiple importance sampling (Direct light + BRDF)
        SampledLight sampled_light;
        if (light_sampler.Sample(&sampled_light, isect, sampler.Next1D()))
        {
            const Light* light = sampled_light.light;
            Float light_weight = sampled_light.weight;

            LightSample ls = light->Sample(isect, sampler.Next2D());

            // Importance sample light
            Float light_brdf_pdf = spdf->Evaluate(ls.wi);
            if (ls.li.IsBlack() == false && light_brdf_pdf > 0)
            {
                Ray shadow_ray(isect.point, ls.wi);
                if (IntersectAny(shadow_ray, Ray::epsilon, ls.visibility) == false)
                {
                    Spectrum f_cos = mat->Evaluate(isect, ray.d, ls.wi);
                    if (light->IsDeltaLight())
                    {
                        L += throughput * light_weight * ls.li * f_cos / ls.pdf;
                    }
                    else
                    {
                        Float mis_weight = PowerHeuristic(1, ls.pdf, 1, light_brdf_pdf);
                        L += throughput * light_weight * mis_weight * ls.li * f_cos / ls.pdf;
                    }
                }
            }

            if (light->IsDeltaLight() == false)
            {
                // Importance sample BRDF
                Vec3 scattered = spdf->Sample(sampler.Next2D());
                Ray shadow_ray(isect.point, scattered);

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

                            Spectrum li = shadow_is.material->Emit(shadow_is, scattered);
                            if (li.IsBlack() == false)
                            {
                                Spectrum f_cos = mat->Evaluate(isect, ray.d, scattered);
                                L += throughput * light_weight * mis_weight * li * f_cos / brdf_pdf;
                            }
                        }
                    }
                    else
                    {
                        Spectrum li = light->Emit(shadow_ray);
                        if (li.IsBlack() == false)
                        {
                            Float brdf_pdf = spdf->Evaluate(scattered);
                            Float mis_weight = PowerHeuristic(1, brdf_pdf, 1, brdf_light_pdf);

                            Spectrum f_cos = mat->Evaluate(isect, ray.d, scattered);
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

        throughput *= mat->Evaluate(isect, ray.d, wi) / pdf;
        ray = Ray(isect.point, wi);

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
