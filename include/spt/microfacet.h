#pragma once

#include "brdf.h"
#include "material.h"

namespace spt
{

// Microfacet material
// Microfacet BRDF (Cook-Torrance specular + Lambertian diffuse)
// GGX normal distribution function
// Smith-GGX height-correlated visibility function
// Schlick Fresnel function
class Microfacet : public Material
{
public:
    Microfacet() = default;

    virtual Spectrum Emit(const Intersection& is, const Vec3& wi) const override;
    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi) const override;
    virtual Spectrum Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const override;

public:
    Ref<Texture> basecolor, metallic, roughness, emissive;
    Ref<Texture> normal_map;
};

inline Spectrum Microfacet::Emit(const Intersection& is, const Vec3& wi) const
{
    return emissive->Evaluate(is.uv);
}

} // namespace spt
