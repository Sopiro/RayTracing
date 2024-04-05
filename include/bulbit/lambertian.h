#pragma once

#include "material.h"
#include "pdf.h"

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

} // namespace bulbit
