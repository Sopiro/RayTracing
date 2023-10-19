#pragma once

#include "brdf.h"
#include "material.h"

namespace bulbit
{

// Microfacet material
// Microfacet BRDF (Cook-Torrance specular + Lambertian diffuse)
// GGX normal distribution function
// Smith-GGX height-correlated visibility function
// Schlick Fresnel function
class Microfacet : public Material
{
public:
    Microfacet(const Ref<Texture> basecolor,
               const Ref<Texture> metallic,
               const Ref<Texture> roughness,
               const Ref<Texture> emissive = ConstantColor::Create(Float(0.0)),
               const Ref<Texture> normalmap = ConstantColor::Create(Float(0.5), Float(0.5), Float(1.0)));

    virtual Spectrum Emit(const Intersection& is, const Vec3& wi) const override;
    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi) const override;
    virtual Spectrum Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const override;

private:
    Ref<Texture> basecolor, metallic, roughness, emissive, normalmap;
};

inline Spectrum Microfacet::Emit(const Intersection& is, const Vec3& wi) const
{
    return emissive->Evaluate(is.uv);
}

} // namespace bulbit
