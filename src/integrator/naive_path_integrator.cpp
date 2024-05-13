#include "bulbit/integrator.h"
#include "bulbit/material.h"

namespace bulbit
{

NaivePathIntegrator::NaivePathIntegrator(
    const Scene* scene, const Intersectable* accel, const Sampler* sampler, int32 bounces, Float rr)
    : SamplerIntegrator(scene, accel, sampler)
    , max_bounces{ bounces }
    , rr_probability{ rr }
{
    for (Light* light : scene->GetLights())
    {
        if (light->type == Light::Type::infinite_light)
        {
            infinite_lights.push_back(light);
        }
    }
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

    const Material* mat = isect.primitive->GetMaterial();

    L += mat->Le(isect, ray.d);

    int8 mem[128];
    Resource res(mem, sizeof(mem));
    Allocator alloc(&res);
    BSDF bsdf;
    if (mat->GetBSDF(&bsdf, isect, ray.d, sampler.Next2D(), alloc) == false)
    {
        return L;
    }

    BSDFSample bsdf_sample;
    if (bsdf.Sample_f(&bsdf_sample, -ray.d, sampler.Next1D(), sampler.Next2D()))
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
