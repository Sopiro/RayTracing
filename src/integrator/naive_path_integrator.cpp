#include "bulbit/material.h"
#include "bulbit/path_integrator.h"

namespace bulbit
{

NaivePathIntegrator::NaivePathIntegrator(const Ref<Sampler> sampler, int32 bounces, Float rr)
    : SamplerIntegrator(sampler)
    , max_bounces{ bounces }
    , rr_probability{ rr }
{
}

Spectrum NaivePathIntegrator::Li(const Scene& scene, const Ray& ray, Sampler& sampler) const
{
    return Li(scene, ray, sampler, 0);
}

Spectrum NaivePathIntegrator::Li(const Scene& scene, const Ray& ray, Sampler& sampler, int32 depth) const
{
    Spectrum L(0);

    if (depth > max_bounces)
    {
        return L;
    }

    Intersection is;
    bool found_intersection = scene.Intersect(&is, ray, Ray::epsilon, infinity);
    if (found_intersection == false)
    {
        for (auto& light : scene.GetInfiniteAreaLights())
        {
            L += light->Emit(ray);
        }

        return L;
    }

    const Material* mat = scene.GetMaterial(is.material_index);

    L += mat->Emit(is, ray.d);

    Interaction ir;
    if (mat->Scatter(&ir, is, ray.d, sampler.Next2D()) == false)
    {
        return L;
    }

    if (ir.is_specular)
    {
        return ir.attenuation * Li(scene, ir.specular_ray, sampler, depth + 1);
    }

    Frame frame(is.normal);
    Vec3 wi_local = CosineSampleHemisphere(sampler.Next2D());
    Vec3 wi = frame.FromLocal(wi_local);

    Float pdf = CosineSampleHemispherePDF(wi_local.z);

    if (pdf > 0)
    {
        Spectrum f_cos = mat->Evaluate(is, ray.d, wi);
        return L + Li(scene, Ray(is.point, wi), sampler, depth + 1) * f_cos / pdf;
    }
    else
    {
        return L;
    }
}

} // namespace bulbit
