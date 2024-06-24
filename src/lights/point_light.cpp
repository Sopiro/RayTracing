#include "bulbit/light.h"

namespace bulbit
{

PointLight::PointLight(const Point3& position, const Spectrum& intensity)
    : Light(Light::Type::point_light)
    , position{ position }
    , intensity{ intensity }
{
}

LightSample PointLight::Sample_Li(const Intersection& ref, const Point2& u) const
{
    Vec3 d = position - ref.point;
    Float distance = d.Normalize();

    LightSample ls;
    ls.wi = d;
    ls.visibility = distance;
    ls.pdf = 1;
    ls.Li = intensity / (distance * distance);

    return ls;
}

Float PointLight::EvaluatePDF(const Ray& ray) const
{
    assert(false);
    return 0;
}

} // namespace bulbit
