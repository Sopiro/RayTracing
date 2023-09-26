#pragma once

#include "light.h"

namespace spt
{

class PointLight : public Light
{
public:
    PointLight(const Point3& position, const Color& intensity);

    virtual Color Sample(Vec3* wi, f64* pdf, f64* visibility, const Intersection& ref) const override;
    virtual f64 EvaluatePDF(const Ray& ray) const override;

    Point3 position;
    Color intensity; // radiance
};

inline f64 PointLight::EvaluatePDF(const Ray& ray) const
{
    assert(false);
    return 0.0;
}

} // namespace spt
