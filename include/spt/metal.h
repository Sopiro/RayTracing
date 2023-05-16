#pragma once

#include "material.h"

namespace spt
{

// The ideal reflector
class Metal : public Material
{
public:
    Metal(const Color& albedo, float64 fuzziness);

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const override;

public:
    Color albedo;
    float64 fuzziness;
};

inline Metal::Metal(const Color& _albedo, float64 _fuzziness)
    : albedo{ _albedo }
    , fuzziness{ _fuzziness }
{
}

inline bool Metal::Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const
{
    Vec3 reflected = Reflect(in_ray.dir.Normalized(), in_rec.normal);

    out_srec.specular_ray = Ray{ in_rec.point, reflected + fuzziness * RandomInUnitSphere() };
    out_srec.attenuation = albedo;
    out_srec.is_specular = true;
    out_srec.pdf = nullptr;

    return true;
}

} // namespace spt
