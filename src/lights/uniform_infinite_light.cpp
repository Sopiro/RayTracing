#include "bulbit/light.h"

namespace bulbit
{

UniformInfiniteLight::UniformInfiniteLight(const Spectrum& l, Float scale)
    : Light(Light::Type::infinite_light)
    , l{ l }
    , scale{ scale }
{
}

LightSample UniformInfiniteLight::Sample_Li(const Intersection& ref, const Point2& u) const
{
    LightSample ls;
    ls.wi = UniformSampleSphere(u);
    ls.pdf = UniformSampleSpherePDF();
    ls.visibility = infinity;
    ls.Li = scale * l;

    return ls;
}

Float UniformInfiniteLight::EvaluatePDF(const Ray& ray) const
{
    return UniformSampleSpherePDF();
}

Spectrum UniformInfiniteLight::Le(const Ray& ray) const
{
    return scale * l;
}

} // namespace bulbit
