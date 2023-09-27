#pragma once

#include "light.h"

namespace spt
{

struct DirectionalLight : public Light
{
public:
    DirectionalLight(const Vec3& dir, const Color& intensity, Float radius);

    virtual Color Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref) const override;
    virtual Float EvaluatePDF(const Ray& ray) const override;

    Vec3 dir;
    Color intensity; // radiance
    Float radius;      // visible radius
};

inline Float DirectionalLight::EvaluatePDF(const Ray& ray) const
{
    assert(false);
    return 0.0;
}

} // namespace spt
