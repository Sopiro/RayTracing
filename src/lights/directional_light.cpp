#include "spt/directional_light.h"
#include "spt/sampling.h"

namespace spt
{

DirectionalLight::DirectionalLight(const Vec3& _dir, const Color& _intensity, Float _radius)
    : Light(Light::Type::directional_light)
    , dir{ Normalize(_dir) }
    , intensity{ _intensity }
    , radius{ _radius }
{
}

Color DirectionalLight::Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref) const
{
    *wi = -dir + RandomInUnitSphere() * radius;
    *pdf = 1.0;
    *visibility = infinity;

    return intensity;
}

} // namespace spt
