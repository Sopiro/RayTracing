#pragma once

#include "material.h"

namespace spt
{

// The ideal reflector
class Metal : public Material
{
public:
    Metal(const Color& albedo, f64 fuzziness);

    virtual bool Scatter(const Ray& in_wi, const Intersection& in_is, Interaction& out_ir) const override;

public:
    Color albedo;
    f64 fuzziness;
};

inline Metal::Metal(const Color& _albedo, f64 _fuzziness)
    : albedo{ _albedo }
    , fuzziness{ _fuzziness }
{
}

inline bool Metal::Scatter(const Ray& in_wi, const Intersection& in_is, Interaction& out_ir) const
{
    Vec3 reflected = Reflect(in_wi.dir.Normalized(), in_is.normal);

    out_ir.specular_ray = Ray{ in_is.point, reflected + fuzziness * RandomInUnitSphere() };
    out_ir.attenuation = albedo;
    out_ir.is_specular = true;
    out_ir.pdf = nullptr;

    return true;
}

} // namespace spt
