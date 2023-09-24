#pragma once

#include "material.h"

namespace spt
{

// Default reflectance of dielectrics
constexpr Vec3 default_reflectance{ 0.04 };
constexpr f64 min_alpha = 0.005;

inline f64 RoughnessToAlpha(f64 roughness)
{
    // return std::fmax(roughness * roughness, min_alpha);
    return std::fmax(roughness, min_alpha);
}

inline Vec3 F0(Vec3 basecolor, f64 metallic)
{
    return Lerp(default_reflectance, basecolor, metallic);
}

inline Vec3 F_Schlick(Vec3 f0, f64 cosine_theta)
{
    return f0 + (Vec3(1.0) - f0) * std::pow(1.0 - cosine_theta, 5.0);
}

inline f64 D_GGX(f64 NoH, f64 alpha2)
{
    f64 NoH2 = NoH * NoH;
    f64 b = (NoH2 * (alpha2 - 1.0) + 1.0);
    return alpha2 * inv_pi / (b * b);
}

inline f64 G1_Smith(f64 NoV, f64 alpha2)
{
    f64 denomC = std::sqrt(alpha2 + (1.0f - alpha2) * NoV * NoV) + NoV;
    return 2.0f * NoV / denomC;
}

inline f64 G2_Smith(f64 NoV, f64 NoL, f64 alpha2)
{
    f64 denomA = NoV * std::sqrt(alpha2 + (1.0 - alpha2) * NoL * NoL);
    f64 denomB = NoL * std::sqrt(alpha2 + (1.0 - alpha2) * NoV * NoV);
    return 2.0 * NoL * NoV / (denomA + denomB);
}

// https://google.github.io/filament/Filament.html#materialsystem/specularbrdf/geometricshadowing(specularg)
inline f64 V_SmithGGXCorrelated(f64 NoV, f64 NoL, f64 alpha2)
{
    f64 GGXV = NoL * std::sqrt(NoV * NoV * (1.0 - alpha2) + alpha2);
    f64 GGXL = NoV * std::sqrt(NoL * NoL * (1.0 - alpha2) + alpha2);
    return 0.5 / (GGXV + GGXL);
}

inline f64 V_SmithGGXCorrelatedFast(f64 NoV, f64 NoL, f64 alpha)
{
    f64 GGXV = NoL * (NoV * (1.0 - alpha) + alpha);
    f64 GGXL = NoV * (NoL * (1.0 - alpha) + alpha);
    return 0.5 / (GGXV + GGXL);
}

// Microfacet material
// Microfacet BRDF (Cook-Torrance specular + Lambertian diffuse)
// GGX normal distribution function
// Smith-GGX height-correlated visibility function
// Schlick Fresnel function
class Microfacet : public Material
{
public:
    Microfacet() = default;

    virtual Color Emit(const Intersection& is, const Vec3& wi) const override;
    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi) const override;
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
