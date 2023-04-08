#pragma once

#include "material.h"

namespace spt
{

class Isotropic : public Material
{
public:
    Isotropic(const Color& color);
    Isotropic(const Ref<Texture>& albedo);

    virtual bool Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const override;

public:
    Ref<Texture> albedo;
};

inline Isotropic::Isotropic(const Color& c)
    : albedo(SolidColor::Create(c))
{
}

inline Isotropic::Isotropic(const Ref<Texture>& a)
    : albedo(a)
{
}

inline bool Isotropic::Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const
{
    out_srec.is_specular = true;
    out_srec.pdf = nullptr;
    out_srec.attenuation = albedo->Value(in_rec.uv, in_rec.point);
    out_srec.specular_ray = Ray{ in_rec.point, RandomInUnitSphere() };

    return true;
}

} // namespace spt
