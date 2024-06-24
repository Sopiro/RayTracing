#include "bulbit/light.h"
#include "bulbit/sampling.h"

namespace bulbit
{

DirectionalLight::DirectionalLight(const Vec3& dir, const Spectrum& intensity, Float radius)
    : Light(Light::Type::directional_light)
    , dir{ Normalize(dir) }
    , intensity{ intensity }
    , radius{ radius }
{
}

LightSample DirectionalLight::Sample_Li(const Intersection& ref, const Point2& u) const
{
    LightSample ls;
    ls.wi = -dir + RandomInUnitSphere(u) * radius;
    ls.pdf = 1;
    ls.visibility = infinity;
    ls.Li = intensity;

    return ls;
}

Float DirectionalLight::EvaluatePDF(const Ray& ray) const
{
    assert(false);
    return 0;
}

} // namespace bulbit
