#include "bulbit/lights.h"

namespace bulbit
{

UniformInfiniteLight::UniformInfiniteLight(const Spectrum& l, Float scale)
    : Light(TypeIndexOf<UniformInfiniteLight>())
    , l{ l }
    , scale{ scale }
{
}

LightSample UniformInfiniteLight::Sample_Li(const Intersection& ref, const Point2& u) const
{
    BulbitNotUsed(ref);

    LightSample light_sample;
    light_sample.wi = SampleUniformSphere(u);
    light_sample.pdf = UniformSampleSpherePDF();
    light_sample.visibility = infinity;
    light_sample.Li = scale * l;

    return light_sample;
}

Float UniformInfiniteLight::EvaluatePDF(const Ray& ray) const
{
    BulbitNotUsed(ray);
    return UniformSampleSpherePDF();
}

Spectrum UniformInfiniteLight::Le(const Ray& ray) const
{
    BulbitNotUsed(ray);
    return scale * l;
}

} // namespace bulbit
