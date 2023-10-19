#pragma once

#include "light.h"

namespace bulbit
{

class PointLight : public Light
{
public:
    PointLight(const Point3& position, const Spectrum& intensity);

    virtual Spectrum Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref) const override;
    virtual Float EvaluatePDF(const Ray& ray) const override;

    Point3 position;
    Spectrum intensity; // radiance
};

inline Float PointLight::EvaluatePDF(const Ray& ray) const
{
    assert(false);
    return Float(0.0);
}

} // namespace bulbit
