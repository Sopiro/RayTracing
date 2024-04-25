#include "bulbit/light.h"

namespace bulbit
{

UniformInfiniteLight::UniformInfiniteLight(const Spectrum& l, Float scale)
    : Light(Light::Type::infinite_area_light)
    , l{ l }
    , scale{ scale }
{
}

Spectrum UniformInfiniteLight::Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref, const Point2& u) const
{
    *wi = UniformSampleSphere(u);
    *pdf = UniformSampleSpherePDF();
    *visibility = infinity;

    return scale * l;
}

Float UniformInfiniteLight::EvaluatePDF(const Ray& ray) const
{
    return UniformSampleSpherePDF();
}

Spectrum UniformInfiniteLight::Emit(const Ray& ray) const
{
    return scale * l;
}

} // namespace bulbit
