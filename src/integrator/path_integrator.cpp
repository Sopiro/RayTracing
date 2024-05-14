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
    int32 bounce = 0;
    Spectrum L(0), throughput(1);
    bool specular_bounce = false;
    Ray ray = primary_ray;

    while (true)
    {
        Intersection isect;
        if (!Intersect(&isect, ray, Ray::epsilon, infinity))
        {
            if (bounce == 0 || specular_bounce)
            {
                for (Light* light : infinite_lights)
                {
                    L += throughput * light->Le(ray);
                }
            }

            break;
        }

        const Material* mat = isect.primitive->GetMaterial();
        Vec3 wo = Normalize(-ray.d);
        if (!mat->IsLightSource() || bounce == 0)
        {
            L += throughput * mat->Le(isect, wo);
        }

        if (bounce++ >= max_bounces)
        {
            break;
        }

        int8 mem[128];
        Resource res(mem, sizeof(mem));
        Allocator alloc(&res);
        BSDF bsdf;
        if (!mat->GetBSDF(&bsdf, isect, wo, alloc))
        {
            break;
        }

        BSDFSample bsdf_sample;
        if (!bsdf.Sample_f(&bsdf_sample, wo, sampler.Next1D(), sampler.Next2D()))
        {
            break;
        }

        specular_bounce = IsSpecular(bsdf_sample.flags);
        ray = Ray(isect.point, bsdf_sample.wi);

        // Estimate direct light
        // Multiple importance sampling (Li + BSDF)
        SampledLight sampled_light;
        if (!specular_bounce && light_sampler.Sample(&sampled_light, isect, sampler.Next1D()))
        {
            const Light* light = sampled_light.light;
            Float light_weight = sampled_light.weight;

            LightSample light_sample = light->Sample(isect, sampler.Next2D());

            // Importance sample light
            Float bsdf_pdf = bsdf.PDF(wo, light_sample.wi);
            if (light_sample.li.IsBlack() == false && bsdf_pdf > 0)
            {
                if (IntersectAny(Ray(isect.point, light_sample.wi), Ray::epsilon, light_sample.visibility) == false)
                {
                    Spectrum f_cos = bsdf.f(wo, light_sample.wi) * Dot(isect.normal, light_sample.wi);
                    if (light->IsDeltaLight())
                    {
                        L += throughput * light_weight * light_sample.li * f_cos / light_sample.pdf;
                    }
                    else
                    {
                        Float mis_weight = PowerHeuristic(1, light_sample.pdf, 1, bsdf_pdf);
                        L += throughput * light_weight * mis_weight * light_sample.li * f_cos / light_sample.pdf;
                    }
                }
            }

            if (light->IsDeltaLight() == false)
            {
                // Importance sample BSDF
                Float light_pdf = light->EvaluatePDF(ray);
                if (light_pdf > 0)
                {
                    Intersection shadow_isect;
                    if (Intersect(&shadow_isect, ray, Ray::epsilon, infinity))
                    {
                        // Intersects area light
                        if (shadow_isect.primitive->GetMaterial()->IsLightSource())
                        {
                            Float mis_weight = PowerHeuristic(1, bsdf_sample.pdf, 1, light_pdf);

                            Spectrum li = shadow_isect.primitive->GetMaterial()->Le(shadow_isect, bsdf_sample.wi);
                            if (li.IsBlack() == false)
                            {
                                Spectrum f_cos = bsdf_sample.f * Dot(isect.normal, bsdf_sample.wi);
                                L += throughput * light_weight * mis_weight * li * f_cos / bsdf_sample.pdf;
                            }
                        }
                    }
                    else
                    {
                        Spectrum li = light->Le(ray);
                        if (li.IsBlack() == false)
                        {
                            Float mis_weight = PowerHeuristic(1, bsdf_sample.pdf, 1, light_pdf);

                            Spectrum f_cos = bsdf_sample.f * Dot(isect.normal, bsdf_sample.wi);
                            L += throughput * light_weight * mis_weight * li * f_cos / bsdf_sample.pdf;
                        }
                    }
                }
            }
        }

        throughput *= bsdf_sample.f * Dot(isect.normal, bsdf_sample.wi) / bsdf_sample.pdf;

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
