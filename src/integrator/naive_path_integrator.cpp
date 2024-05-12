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

    Intersection is;
    bool found_intersection = Intersect(&is, ray, Ray::epsilon, infinity);
    if (found_intersection == false)
    {
        for (auto& light : infinite_lights)
        {
            L += light->Emit(ray);
        }

        return L;
    }

    const Material* mat = is.material;

    L += mat->Emit(is, ray.d);

    Interaction ir;
    if (mat->Scatter(&ir, is, ray.d, sampler.Next2D()) == false)
    {
        return L;
    }

    BSDFSample bsdf_sample;
    if (ir.bsdf.Sample_f(&bsdf_sample, -ray.d, sampler.Next1D(), sampler.Next2D()))
    {
        Spectrum f_cos = bsdf_sample.f * Dot(is.normal, bsdf_sample.wi);

        return L + Li(Ray(is.point, bsdf_sample.wi), sampler, depth + 1) * f_cos / bsdf_sample.pdf;
    }
    else
    {
        return L;
    }
}

} // namespace bulbit
