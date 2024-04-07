#include "bulbit/frame.h"
#include "bulbit/integrator.h"

namespace bulbit
{

AmbientOcclusion::AmbientOcclusion(const Ref<Sampler> sampler, Float ao_range)
    : SamplerIntegrator(sampler)
    , range{ ao_range }
{
}

Spectrum AmbientOcclusion::Li(const Scene& scene, const Ray& primary_ray, Sampler& sampler) const
{
    Intersection is;
    bool found_intersection = scene.Intersect(&is, primary_ray, Ray::epsilon, infinity);

    if (found_intersection == false)
    {
        return Spectrum::black;
    }

    Vec3 wi_local = CosineSampleHemisphere(sampler.Next2D());
    Float pdf = CosineSampleHemispherePDF(wi_local.z);

    if (wi_local.z <= 0)
    {
        return Spectrum::black;
    }

    Frame frame(is.normal);
    Vec3 wi = frame.FromLocal(wi_local);

    Ray ao_ray(is.point, wi);
    if (scene.IntersectAny(ao_ray, Ray::epsilon, range) == false)
    {
#if 0
        return Spectrum(wi_local.z * inv_pi / pdf);
#else
        return Spectrum(1); // importance sampling cancels all terms!
#endif
    }
    else
    {
        return Spectrum::black;
    }
}

} // namespace bulbit
