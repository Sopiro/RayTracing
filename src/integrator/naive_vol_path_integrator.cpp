#include "bulbit/bxdfs.h"
#include "bulbit/integrator.h"
#include "bulbit/material.h"

namespace bulbit
{

NaiveVolPathIntegrator::NaiveVolPathIntegrator(
    const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler, int32 max_bounces, Float rr_probability)
    : SamplerIntegrator(accel, std::move(lights), sampler)
    , max_bounces{ max_bounces }
    , rr_probability{ rr_probability }
{
    for (Light* light : all_lights)
    {
        switch (light->type)
        {
        case Light::Type::infinite_light:
        {
            infinite_lights.push_back(light);
        }
        break;
        case Light::Type::area_light:
        {
            AreaLight* area_light = (AreaLight*)light;
            area_lights.emplace(area_light->GetPrimitive(), area_light);
        }
        break;
        default:
            break;
        }
    }
}

Spectrum NaiveVolPathIntegrator::Li(const Ray& primary_ray, Sampler& sampler) const
{
    int32 bounce = 0;
    Spectrum L(0), throughput(1);
    Float eta_scale = 1;
    Ray ray = primary_ray;

    while (true)
    {
        Intersection isect;
        if (!Intersect(&isect, ray, Ray::epsilon, infinity))
        {
            for (Light* light : infinite_lights)
            {
                L += throughput * light->Le(ray);
            }

            break;
        }

        if (bounce++ >= max_bounces)
        {
            break;
        }

        Vec3 wo = Normalize(-ray.d);

        // Incorporate surface emission
        L += throughput * isect.Le(wo);

        int8 mem[max_bxdf_size];
        Resource res(mem, sizeof(mem));
        Allocator alloc(&res);
        BSDF bsdf;
        if (isect.GetBSDF(&bsdf, wo, alloc) == false)
        {
            break;
        }

        BSDFSample bsdf_sample;
        if (!bsdf.Sample_f(&bsdf_sample, wo, sampler.Next1D(), sampler.Next2D()))
        {
            break;
        }

        if (bsdf_sample.IsTransmission())
        {
            eta_scale *= Sqr(bsdf_sample.eta);
        }

        throughput *= bsdf_sample.f * AbsDot(isect.shading.normal, bsdf_sample.wi) / bsdf_sample.pdf;
        ray = Ray(isect.point, bsdf_sample.wi);

        // Terminate path with russian roulette
        constexpr int32 min_bounces = 2;
        if (bounce > min_bounces)
        {
            Float rr = std::fmin(rr_probability, throughput.Luminance() * eta_scale);
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
