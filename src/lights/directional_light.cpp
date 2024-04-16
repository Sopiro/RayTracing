#include "bulbit/light.h"
#include "bulbit/sampling.h"

namespace bulbit
{

DirectionalLight::DirectionalLight(const Vec3& _dir, const Spectrum& _intensity, Float _radius)
    : Light(Light::Type::directional_light)
    , dir{ Normalize(_dir) }
    , intensity{ _intensity }
    , radius{ _radius }
{
}

Spectrum DirectionalLight::Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref, const Point2& u) const
{
    *wi = -dir + RandomInUnitSphere(u) * radius;
    *pdf = Float(1.0);
    *visibility = infinity;

    return intensity;
}

} // namespace bulbit
