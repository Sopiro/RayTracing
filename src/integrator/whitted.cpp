#include "bulbit/bxdfs.h"
#include "bulbit/integrator.h"
#include "bulbit/material.h"

namespace bulbit
{

WhittedStyle::WhittedStyle(const Scene* scene, const Intersectable* accel, const Sampler* sampler, int32 max_depth)
    : SamplerIntegrator(scene, accel, sampler)
    , max_depth{ max_depth }
{
    for (Light* light : scene->GetLights())
    {
        if (light->type == Light::Type::infinite_light)
        {
            infinite_lights.push_back(light);
        }
    }
}

Spectrum WhittedStyle::Li(const Ray& ray, Sampler& sampler, int32 depth) const
{
    Spectrum L(0);

    if (depth > max_depth)
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
    Vec3 wo = Normalize(-ray.d);

    // Evaluate emitted light
    L += mat->Le(isect, wo);

    int8 mem[max_bxdf_size];
    Resource res(mem, sizeof(mem));
    Allocator alloc(&res);
    BSDF bsdf;
    if (mat->GetBSDF(&bsdf, isect, wo, alloc) == false)
    {
        return L;
    }

    // Evaluate direct light
    const std::vector<Light*>& lights = scene->GetLights();
    for (const Light* light : lights)
    {
        LightSample light_sample = light->Sample_Li(isect, sampler.Next2D());
        if (light_sample.Li.IsBlack() == false && light_sample.pdf > 0)
        {
            Ray shadow_ray{ isect.point, light_sample.wi };
            if (IntersectAny(shadow_ray, Ray::epsilon, light_sample.visibility) == false)
            {
                Spectrum f_cos = bsdf.f(wo, light_sample.wi);
                L += light_sample.Li * f_cos / light_sample.pdf;
            }
        }
    }

    // Evaluate indirect light only for the perfect reflection direction
    Vec3 wi = Reflect(wo, isect.normal);

    if (Dot(isect.normal, wi) > 0)
    {
        Spectrum f_cos = bsdf.f(wo, wi);
        return L + Li(Ray(isect.point, wi), sampler, depth + 1) * f_cos;
    }
    else
    {
        return L;
    }
}

} // namespace bulbit
