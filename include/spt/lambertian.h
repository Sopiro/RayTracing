#pragma once

#include "cosine_pdf.h"
#include "material.h"

namespace spt
{

class Lambertian : public Material
{
public:
    Lambertian(const Color& color);
    Lambertian(const Ref<Texture>& albedo);

    virtual bool Scatter(const Intersection& is, const Ray& wi, Interaction& out_ir) const override;
    virtual Vec3 Evaluate(const Intersection& is, const Ray& wi, const Ray& wo) const override;

public:
    Ref<Texture> albedo;
};

inline Lambertian::Lambertian(const Color& _color)
    : albedo{ SolidColor::Create(_color) }
{
}

inline Lambertian::Lambertian(const Ref<Texture>& _albedo)
    : albedo{ _albedo }
{
}

inline bool Lambertian::Scatter(const Intersection& is, const Ray& wi, Interaction& out_ir) const
{
    out_ir.is_specular = false;
    out_ir.attenuation = albedo->Value(is.uv, is.point);
    out_ir.pdf = CreateSharedRef<CosinePDF>(is.normal);

    return true;
}

inline Vec3 Lambertian::Evaluate(const Intersection& is, const Ray& wi, const Ray& wo) const
{
    return albedo->Value(is.uv, is.point) * Dot(is.normal, wo.dir) * inv_pi;
}

} // namespace spt
