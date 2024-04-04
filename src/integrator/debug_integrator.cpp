#include "bulbit/integrator.h"

namespace bulbit
{

DebugIntegrator::DebugIntegrator(const Ref<Sampler> sampler)
    : SamplerIntegrator(sampler)
{
}

Spectrum DebugIntegrator::Li(const Scene& scene, const Ray& primary_ray, Sampler& sampler) const
{
    Intersection is;
    bool found_intersection = scene.Intersect(&is, primary_ray, Ray::epsilon, infinity);

    if (found_intersection == false)
    {
        return Spectrum::black;
    }

    const Material* mat = scene.GetMaterial(is.material_index);

    Vec3 wi = Normalize(primary_ray.d);
    Vec3 wo = Normalize(Reflect(wi, is.normal));

    return Spectrum(1 / (1 + is.t));
}

} // namespace bulbit
