#pragma once

#include "material.h"

namespace spt
{

// The ideal reflector
class Metal : public Material
{
public:
    Metal(const Color& albedo, f64 fuzziness);

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Ray& wi) const override;

public:
    Color albedo;
    f64 fuzziness;
};

inline Metal::Metal(const Color& _albedo, f64 _fuzziness)
    : albedo{ _albedo }
    , fuzziness{ _fuzziness }
{
}

inline bool Metal::Scatter(Interaction* ir, const Intersection& is, const Ray& wi) const
{
    Vec3 reflected = Reflect(wi.dir.Normalized(), is.normal);

    ir->specular_ray = Ray{ is.point, reflected + fuzziness * RandomInUnitSphere() };
    ir->attenuation = albedo;
    ir->is_specular = true;
    ir->pdf = nullptr;

    return true;
}

} // namespace spt
