#include "bulbit/bxdfs.h"
#include "bulbit/integrators.h"
#include "bulbit/lights.h"
#include "bulbit/material.h"

namespace bulbit
{

PathIntegrator::PathIntegrator(
    const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler, int32 max_bounces, bool regularize_bsdf
)
    : UniDirectionalRayIntegrator(accel, std::move(lights), sampler)
    , max_bounces{ max_bounces }
    , regularize_bsdf{ regularize_bsdf }
    , light_sampler{ all_lights }
{
    for (Light* light : all_lights)
    {
        switch (light->GetTypeIndex())
        {
        case Light::TypeIndexOf<UniformInfiniteLight>():
        case Light::TypeIndexOf<ImageInfiniteLight>():
        {
            infinite_lights.push_back(light);
        }
        break;
        case Light::TypeIndexOf<AreaLight>():
        {
            AreaLight* area_light = light->Cast<AreaLight>();
            area_lights.emplace(area_light->GetPrimitive(), area_light);
        }
        break;
        default:
            break;
        }
    }
}

Spectrum PathIntegrator::Li(const Ray& primary_ray, const Medium* primary_medium, Sampler& sampler) const
{
    int32 bounce = 0;
    Spectrum L(0), beta(1);
    bool specular_bounce = false;
    bool any_non_specular_bounces = false;
    Float eta_scale = 1;
    Ray ray = primary_ray;
    Float prev_bsdf_pdf = 0;

    while (true)
    {
        Intersection isect;
        if (!Intersect(&isect, ray, Ray::epsilon, infinity))
        {
            if (bounce == 0 || specular_bounce)
            {
                for (Light* light : infinite_lights)
                {
                    L += beta * light->Le(ray);
                }
            }
            else
            {
                // Evaluate BSDF sample MIS for infinite light
                for (Light* light : infinite_lights)
                {
                    Float light_pdf = light->EvaluatePDF(ray) * light_sampler.EvaluatePMF(light);
                    Float mis_weight = PowerHeuristic(1, prev_bsdf_pdf, 1, light_pdf);

                    L += beta * mis_weight * light->Le(ray);
                }
            }

            break;
        }

        Vec3 wo = Normalize(-ray.d);

        if (Spectrum Le = isect.Le(wo); !Le.IsBlack())
        {
            bool has_area_light = area_lights.contains(isect.primitive);
            if (bounce == 0 || specular_bounce || !has_area_light)
            {
                L += beta * Le;
            }
            else if (has_area_light)
            {
                // Evaluate BSDF sample with MIS for area light
                AreaLight* area_light = area_lights.at(isect.primitive);

                Float light_pdf = isect.primitive->GetShape()->PDF(isect, ray) * light_sampler.EvaluatePMF(area_light);
                Float mis_weight = PowerHeuristic(1, prev_bsdf_pdf, 1, light_pdf);

                L += beta * mis_weight * Le;
            }
        }

        if (bounce++ >= max_bounces)
        {
            break;
        }

        int8 mem[max_bxdf_size];
        Resource res(mem, sizeof(mem));
        Allocator alloc(&res);
        BSDF bsdf;
        if (!isect.GetBSDF(&bsdf, wo, alloc))
        {
            ray = Ray(isect.point, -wo);
            --bounce;
            continue;
        }

        // Blur bsdf if possible
        if (regularize_bsdf && any_non_specular_bounces)
        {
            bsdf.Regularize();
        }

        // Estimate direct light
        if (IsNonSpecular(bsdf.Flags()))
        {
            L += SampleDirectLight(wo, isect, &bsdf, sampler, beta);
        }

        BSDFSample bsdf_sample;
        if (!bsdf.Sample_f(&bsdf_sample, wo, sampler.Next1D(), sampler.Next2D()))
        {
            break;
        }

        specular_bounce = bsdf_sample.IsSpecular();
        any_non_specular_bounces |= !bsdf_sample.IsSpecular();
        if (bsdf_sample.IsTransmission())
        {
            eta_scale *= Sqr(bsdf_sample.eta);
        }

        // Save bsdf pdf for MIS
        prev_bsdf_pdf = bsdf_sample.pdf;
        beta *= bsdf_sample.f * AbsDot(isect.shading.normal, bsdf_sample.wi) / bsdf_sample.pdf;
        ray = Ray(isect.point, bsdf_sample.wi);

        // Terminate path with russian roulette
        constexpr int32 min_bounces = 2;
        if (bounce > min_bounces)
        {
            Float p = beta.MaxComponent() * eta_scale;
            if (p < 1 && sampler.Next1D() > p)
            {
                break;
            }

            beta /= p;
        }
    }

    return L;
}

Spectrum PathIntegrator::SampleDirectLight(
    const Vec3& wo, const Intersection& isect, BSDF* bsdf, Sampler& sampler, const Spectrum& beta
) const
{
    Float u0 = sampler.Next1D();
    Point2 u12 = sampler.Next2D();
    SampledLight sampled_light;
    if (!light_sampler.Sample(&sampled_light, isect, u0))
    {
        return Spectrum::black;
    }

    LightSample light_sample = sampled_light.light->Sample_Li(isect, u12);
    Float bsdf_pdf = bsdf->PDF(wo, light_sample.wi);
    if (light_sample.Li.IsBlack() || bsdf_pdf == 0)
    {
        return Spectrum::black;
    }

    Ray shadow_ray(isect.point, light_sample.wi);
    if (IntersectAny(shadow_ray, Ray::epsilon, light_sample.visibility))
    {
        return Spectrum::black;
    }

    Float light_pdf = light_sample.pdf / sampled_light.weight;
    Spectrum f_cos = bsdf->f(wo, light_sample.wi) * AbsDot(isect.shading.normal, light_sample.wi);

    if (sampled_light.light->IsDeltaLight())
    {
        return beta * light_sample.Li * f_cos / light_pdf;
    }
    else
    {
        Float mis_weight = PowerHeuristic(1, light_pdf, 1, bsdf_pdf);
        return beta * mis_weight * light_sample.Li * f_cos / light_pdf;
    }
}

} // namespace bulbit
