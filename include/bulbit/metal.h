#pragma once

#include "material.h"

namespace bulbit
{

// Conductor
class Metal : public Material
{
public:
    Metal(const Spectrum& albedo, Float fuzziness);

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const override;

public:
    Spectrum albedo;
    Float fuzziness;
};

inline Metal::Metal(const Spectrum& _albedo, Float _fuzziness)
    : albedo{ _albedo }
    , fuzziness{ _fuzziness }
{
}

inline bool Metal::Scatter(Interaction* ir, const Intersection& is, const Vec3& wi, const Point2& u) const
{
    Vec3 reflected = Reflect(-wi, is.normal);

    ir->specular_ray.o = is.point;
    ir->specular_ray.d = Normalize(reflected + fuzziness * RandomInUnitSphere(u));
    ir->attenuation = albedo;
    ir->is_specular = true;

    return true;
}

} // namespace bulbit
