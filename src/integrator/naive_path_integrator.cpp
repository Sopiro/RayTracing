#include "bulbit/bxdfs.h"
#include "bulbit/integrator.h"
#include "bulbit/material.h"

namespace bulbit
{

NaivePathIntegrator::NaivePathIntegrator(
    const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler, int32 bounces, Float rr
)
    : SamplerIntegrator(accel, std::move(lights), sampler)
    , max_bounces{ bounces }
    , rr_probability{ rr }
{
    for (Light* light : all_lights)
    {
        if (light->type == Light::Type::infinite_light)
        {
            infinite_lights.push_back(light);
        }
    }
}

Spectrum NaivePathIntegrator::Li(const Ray& primary_ray, Sampler& sampler) const
{
    // return Li(ray, sampler, 0);

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
            ray = Ray(isect.point, -wo);
            continue;
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

Spectrum NaivePathIntegrator::Li(const Ray& ray, Sampler& sampler, int32 depth) const
{
    Spectrum L(0);

    if (depth > max_bounces)
    {
        return L;
    }

    Intersection isect;
    if (!Intersect(&isect, ray, Ray::epsilon, infinity))
    {
        for (auto& light : infinite_lights)
        {
            L += light->Le(ray);
        }

        return L;
    }

    Vec3 wo = Normalize(-ray.d);

    L += isect.Le(wo);

    int8 mem[max_bxdf_size];
    Resource res(mem, sizeof(mem));
    Allocator alloc(&res);
    BSDF bsdf;
    if (isect.GetBSDF(&bsdf, wo, alloc) == false)
    {
        return L;
    }

    BSDFSample bsdf_sample;
    if (bsdf.Sample_f(&bsdf_sample, wo, sampler.Next1D(), sampler.Next2D()))
    {
        Spectrum f_cos = bsdf_sample.f * Dot(isect.normal, bsdf_sample.wi);
        return L + Li(Ray(isect.point, bsdf_sample.wi), sampler, depth + 1) * f_cos / bsdf_sample.pdf;
    }
    else
    {
        return L;
    }
}

} // namespace bulbit
