#include "bulbit/lights.h"
#include "bulbit/sampling.h"

namespace bulbit
{

DirectionalLight::DirectionalLight(const Vec3& dir, const Spectrum& intensity, Float radius)
    : Light(TypeIndexOf<DirectionalLight>())
    , dir{ Normalize(dir) }
    , intensity{ intensity }
    , radius{ radius }
{
}

LightSample DirectionalLight::Sample_Li(const Intersection& ref, const Point2& u) const
{
    BulbitNotUsed(ref);

    LightSample light_sample;
    light_sample.wi = -dir + SampleInsideUnitSphere(u) * radius;
    light_sample.pdf = 1;
    light_sample.visibility = infinity;
    light_sample.Li = intensity;

    return light_sample;
}

Float DirectionalLight::EvaluatePDF(const Ray& ray) const
{
    BulbitAssert(false);
    BulbitNotUsed(ray);
    return 0;
}

} // namespace bulbit
