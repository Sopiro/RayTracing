#pragma once

#include "light.h"

namespace spt
{

struct DirectionalLight : public Light
{
public:
    DirectionalLight(const Vec3& dir, const Color& intensity, f64 radius);

    virtual Color Sample(Vec3* wi, f64* pdf, f64* visibility, const Intersection& ref) const override;
    virtual f64 EvaluatePDF(const Ray& ray) const override;

    Vec3 dir;
    Color intensity; // radiance
    f64 radius;      // visible radius
};

inline DirectionalLight::DirectionalLight(const Vec3& _dir, const Color& _intensity, f64 _radius)
    : Light(Light::Type::directional_light)
    , dir{ Normalize(_dir) }
    , intensity{ _intensity }
    , radius{ _radius }
{
}

inline Color DirectionalLight::Sample(Vec3* wi, f64* pdf, f64* visibility, const Intersection& ref) const
{
    *wi = -dir + RandomInUnitSphere() * radius;
    *pdf = 1.0;
    *visibility = infinity;

    return intensity;
}

inline f64 DirectionalLight::EvaluatePDF(const Ray& ray) const
{
    assert(false);
    return 0.0;
}

} // namespace spt
