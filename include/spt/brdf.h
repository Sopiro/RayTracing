#pragma once

#include "common.h"

namespace spt
{

// Functions for microfacet BRDF

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
    return f0 + (/*f90*/ Vec3(1.0) - f0) * std::pow(1.0 - cosine_theta, 5.0);
}

// Trowbridge-Reitz distribution
inline f64 D_GGX(f64 NoH, f64 alpha2)
{
    f64 b = (NoH * NoH * (alpha2 - 1.0) + 1.0);
    return alpha2 * inv_pi / (b * b);
}

inline f64 G1_Smith(f64 NoV, f64 alpha2)
{
    return 2.0 * NoV / (NoV + std::sqrt(alpha2 + (1.0 - alpha2) * NoV * NoV));
}

inline f64 G2_Smith_Correlated(f64 NoV, f64 NoL, f64 alpha2)
{
    f64 g1 = NoV * std::sqrt(alpha2 + (1.0 - alpha2) * NoL * NoL);
    f64 g2 = NoL * std::sqrt(alpha2 + (1.0 - alpha2) * NoV * NoV);
    return 2.0 * NoL * NoV / (g1 + g2);
}

inline f64 V_Smith_Correlated(f64 NoV, f64 NoL, f64 alpha2)
{
    f64 g1 = NoV * std::sqrt(alpha2 + (1.0 - alpha2) * NoL * NoL);
    f64 g2 = NoL * std::sqrt(alpha2 + (1.0 - alpha2) * NoV * NoV);
    return 0.5 / (g1 + g2);
}

} // namespace spt
