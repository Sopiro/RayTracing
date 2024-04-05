#include "bulbit/whitted.h"

namespace bulbit
{

WhittedStyle::WhittedStyle(const Ref<Sampler> sampler, int32 max_depth)
    : SamplerIntegrator(sampler)
    , max_depth{ max_depth }
{
}

Spectrum WhittedStyle::Li(const Scene& scene, const Ray& ray, Sampler& sampler, int32 depth) const
{
    Spectrum l(0);

    Intersection is;
    bool found_intersection = scene.Intersect(&is, ray, Ray::epsilon, infinity);
    if (found_intersection == false)
    {
        for (auto& light : scene.GetInfiniteAreaLights())
        {
            l += light->Emit(ray);
        }

        return l;
    }

    const Material* mat = scene.GetMaterial(is.material_index);

    // Evaluate emitted light if ray hit an area light source
    l += mat->Emit(is, ray.d);

    Interaction ir;
    if (mat->Scatter(&ir, is, ray.d, sampler.Next2D()) == false)
    {
        return l;
    }

    if (ir.is_specular)
    {
        return ir.attenuation * Li(scene, ir.specular_ray, sampler, depth + 1);
    }

    // Evaluate direct light
    const std::vector<Ref<Light>>& lights = scene.GetLights();
    for (auto light : lights)
    {
        Vec3 to_light;
        Float light_pdf;
        Float visibility;
        Spectrum li = light->Sample(&to_light, &light_pdf, &visibility, is, sampler.Next2D());

        Ray shadow_ray{ is.point, to_light };

        if (li.IsBlack() == false && light_pdf > 0)
        {
            if (scene.IntersectAny(shadow_ray, Ray::epsilon, visibility) == false)
            {
                Spectrum f_cos = mat->Evaluate(is, ray.d, to_light);
                l += li * f_cos / light_pdf;
            }
        }
    }

    if (depth + 1 < max_depth)
    {
        // Evaluate indirect light only in the perfect reflection direction
        Vec3 wi = Reflect(-ray.d, is.normal);

        if (Dot(is.normal, wi) > 0)
        {
            Spectrum f_cos = mat->Evaluate(is, ray.d, wi);
            return l + Li(scene, Ray(is.point, wi), sampler, depth + 1) * f_cos;
        }
        else
        {
            return l;
        }
    }
    else
    {
        return l;
    }
}

} // namespace bulbit
