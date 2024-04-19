#include "bulbit/light.h"

namespace bulbit
{

PointLight::PointLight(const Point3& position, const Spectrum& intensity)
    : Light(Light::Type::point_light)
    , position{ position }
    , intensity{ intensity }
{
}

Spectrum PointLight::Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref, const Point2& u) const
{
    Vec3 d = position - ref.point;
    Float distance = d.Normalize();

    *wi = d;
    *visibility = distance;
    *pdf = Float(1.0);

    return intensity / (distance * distance);
}

} // namespace bulbit
