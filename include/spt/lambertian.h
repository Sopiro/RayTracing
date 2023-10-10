#pragma once

#include "cosine_pdf.h"
#include "material.h"

namespace spt
{

class Lambertian : public Material
{
public:
    Lambertian(const Spectrum& color);
    Lambertian(const Ref<Texture> albedo);

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi) const override;
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

inline bool Lambertian::Scatter(Interaction* ir, const Intersection& is, const Vec3& wi) const
{
    ir->is_specular = false;
    ir->attenuation = albedo->Value(is.uv);
    new (ir->mem) CosinePDF(is.normal);

    return true;
}

inline Spectrum Lambertian::Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const
{
    return albedo->Value(is.uv) * Dot(is.normal, wo) * inv_pi;
}

} // namespace spt
