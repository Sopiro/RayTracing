#pragma once

#include "material.h"

namespace spt
{

class Isotropic : public Material
{
public:
    Isotropic(const Color& color);
    Isotropic(const Ref<Texture>& albedo);

    virtual bool Scatter(const Intersection& is, const Ray& wi, Interaction& out_ir) const override;

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

inline bool Isotropic::Scatter(const Intersection& is, const Ray& wi, Interaction& out_ir) const
{
    out_ir.is_specular = true;
    out_ir.pdf = nullptr;
    out_ir.attenuation = albedo->Value(is.uv, is.point);
    out_ir.specular_ray = Ray{ is.point, RandomInUnitSphere() };

    return true;
}

} // namespace spt
