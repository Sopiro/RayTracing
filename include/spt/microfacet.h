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

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi) const override;
    virtual Color Emit(const Intersection& is, const Vec3& wi) const override;
    virtual Vec3 Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const override;

public:
    Ref<Texture> basecolor, metallic, roughness, emissive;
    Ref<Texture> normal_map;
};

inline Color Microfacet::Emit(const Intersection& is, const Vec3& wi) const
{
    return emissive->Value(is.uv);
}

} // namespace spt
