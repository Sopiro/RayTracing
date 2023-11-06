#pragma once

#include "lambertian_pdf.h"
#include "material.h"

namespace bulbit
{

class Lambertian : public Material
{
public:
    Lambertian(const Spectrum& color);
    Lambertian(const Ref<Texture> albedo);

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const override;
    virtual Spectrum Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const override;

public:
    Ref<Texture> albedo;
};

inline Lambertian::Lambertian(const Spectrum& color)
    : albedo{ ConstantColor::Create(color) }
{
}

inline Lambertian::Lambertian(const Ref<Texture> _albedo)
    : albedo{ _albedo }
{
}

inline bool Lambertian::Scatter(Interaction* ir, const Intersection& is, const Vec3& wi, const Point2& u) const
{
    ir->is_specular = false;
    ir->attenuation = albedo->Evaluate(is.uv);
    new (ir->mem) LambertianReflection(is.normal);

    return true;
}

inline Spectrum Lambertian::Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const
{
    return albedo->Evaluate(is.uv) * Dot(is.normal, wo) * inv_pi;
}

} // namespace bulbit
