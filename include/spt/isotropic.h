#pragma once

#include "material.h"

namespace spt
{

class Isotropic : public Material
{
public:
    Isotropic(const Color& color);
    Isotropic(const Ref<Texture> albedo);

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi) const override;

public:
    Ref<Texture> albedo;
};

inline Isotropic::Isotropic(const Color& c)
    : albedo(ConstantColor::Create(c))
{
}

inline Isotropic::Isotropic(const Ref<Texture> a)
    : albedo(a)
{
}

inline bool Isotropic::Scatter(Interaction* ir, const Intersection& is, const Vec3& wi) const
{
    ir->is_specular = true;
    ir->attenuation = albedo->Value(is.uv);
    ir->specular_ray = Ray{ is.point, RandomInUnitSphere() };

    return true;
}

} // namespace spt
