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

    // Evaluate emitted light if ray hit an area light source
    L += mat->Emit(is, ray.d);

    Interaction ir;
    if (mat->Scatter(&ir, is, ray.d, sampler.Next2D()) == false)
    {
        return L;
    }

    if (ir.is_specular)
    {
        return ir.attenuation * Li(ir.specular_ray, sampler, depth + 1);
    }

    // Evaluate direct light
    const std::vector<Light*>& lights = scene->GetLights();
    for (const Light* light : lights)
    {
        LightSample ls = light->Sample(is, sampler.Next2D());
        if (ls.li.IsBlack() == false && ls.pdf > 0)
        {
            Ray shadow_ray{ is.point, ls.wi };
            if (IntersectAny(shadow_ray, Ray::epsilon, ls.visibility) == false)
            {
                Spectrum f_cos = mat->Evaluate(is, ray.d, ls.wi);
                L += ls.li * f_cos / ls.pdf;
            }
        }
    }

    // Evaluate indirect light only in the perfect reflection direction
    Vec3 wi = Reflect(-ray.d, is.normal);

    if (Dot(is.normal, wi) > 0)
    {
        Spectrum f_cos = mat->Evaluate(is, ray.d, wi);
        return L + Li(Ray(is.point, wi), sampler, depth + 1) * f_cos;
    }
    else
    {
        return L;
    }
}

} // namespace bulbit
