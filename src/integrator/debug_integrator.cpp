#include "bulbit/integrator.h"

namespace bulbit
{

DebugIntegrator::DebugIntegrator(const Scene* scene, const Intersectable* accel, const Sampler* sampler)
    : SamplerIntegrator(scene, accel, sampler)
{
}

Spectrum DebugIntegrator::Li(const Ray& primary_ray, Sampler& sampler) const
{
    Intersection is;
    bool found_intersection = Intersect(&is, primary_ray, Ray::epsilon, infinity);

    if (found_intersection == false)
    {
        return Spectrum::black;
    }

    const Material* mat = is.material;

    Vec3 wi = Normalize(primary_ray.d);
    Vec3 wo = Normalize(Reflect(wi, is.normal));

    // return Spectrum(1 / (1 + is.t));
    return Spectrum(is.shading.normal) * 0.5 + Spectrum(0.5);
}

} // namespace bulbit
