#include "spt/directional_light.h"

namespace spt
{

DirectionalLight::DirectionalLight(const Vec3& _dir, const Color& _intensity, f64 _radius)
    : Light(Light::Type::directional_light)
    , dir{ Normalize(_dir) }
    , intensity{ _intensity }
    , radius{ _radius }
{
}

Color DirectionalLight::Sample(Vec3* wi, f64* pdf, f64* visibility, const Intersection& ref) const
{
    *wi = -dir + RandomInUnitSphere() * radius;
    *pdf = 1.0;
    *visibility = infinity;

    return intensity;
}

} // namespace spt
