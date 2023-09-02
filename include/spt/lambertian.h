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

    virtual bool Scatter(const Ray& in_wi, const Intersection& in_is, Interaction& out_ir) const override;
    virtual Vec3 Evaluate(const Ray& in_wi, const Intersection& in_is, const Ray& in_wo) const override;

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

inline bool Lambertian::Scatter(const Ray& in_wi, const Intersection& in_is, Interaction& out_ir) const
{
    out_ir.is_specular = false;
    out_ir.attenuation = albedo->Value(in_is.uv, in_is.point);
    out_ir.pdf = CreateSharedRef<CosinePDF>(in_is.normal);

    return true;
}

inline Vec3 Lambertian::Evaluate(const Ray& in_wi, const Intersection& in_is, const Ray& in_wo) const
{
    return albedo->Value(in_is.uv, in_is.point) * Dot(in_is.normal, in_wo.dir) * inv_pi;
}

} // namespace spt
