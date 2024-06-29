#include "bulbit/frame.h"
#include "bulbit/integrator.h"

namespace bulbit
{

AmbientOcclusion::AmbientOcclusion(const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler, Float ao_range)
    : SamplerIntegrator(accel, std::move(lights), sampler)
    , range{ ao_range }
{
}

Spectrum AmbientOcclusion::Li(const Ray& primary_ray, Sampler& sampler) const
{
    Intersection isect;
    if (!Intersect(&isect, primary_ray, Ray::epsilon, infinity))
    {
        return Spectrum::black;
    }

    Vec3 wi_local = SampleCosineHemisphere(sampler.Next2D());
    Float pdf = CosineHemispherePDF(wi_local.z);

    if (wi_local.z <= 0)
    {
        return Spectrum::black;
    }

    Frame frame(isect.normal);
    Vec3 wi = frame.FromLocal(wi_local);

    Ray ao_ray(isect.point, wi);
    if (IntersectAny(ao_ray, Ray::epsilon, range) == false)
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
