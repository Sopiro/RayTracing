#include "bulbit/integrator.h"

namespace bulbit
{

DebugIntegrator::DebugIntegrator(const Scene* scene, const Intersectable* accel, const Sampler* sampler)
    : SamplerIntegrator(scene, accel, sampler)
{
}

Spectrum DebugIntegrator::Li(const Ray& primary_ray, Sampler& sampler) const
{
    Intersection isect;
    if (!Intersect(&isect, primary_ray, Ray::epsilon, infinity))
    {
        return Spectrum::black;
    }

    const Material* mat = isect.primitive->GetMaterial();

    Vec3 wi = Normalize(primary_ray.d);
    Vec3 wo = Normalize(Reflect(wi, isect.normal));

    // return Spectrum(1 / (1 + isect.t));
    return Spectrum(isect.shading.normal) * 0.5 + Spectrum(0.5);
}

} // namespace bulbit
