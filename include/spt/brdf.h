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

// Importance sampling codes

inline Vec3 Sample_GGX(Vec3 wo, f64 alpha2, Vec2 u)
{
    f64 theta = std::acos(std::sqrt((1.0 - u.x) / ((alpha2 - 1.0) * u.x + 1.0)));
    f64 phi = two_pi * u.y;

    f64 sin_thetha = std::sin(theta);
    f64 x = std::cos(phi) * sin_thetha;
    f64 y = std::sin(phi) * sin_thetha;
    f64 z = std::cos(theta);

    Vec3 h{ x, y, z }; // Sampled half vector

    return h;
}

// "Sampling Visible GGX Normals with Spherical Caps" by Dupuy & Benyoub
// https://gist.github.com/jdupuy/4c6e782b62c92b9cb3d13fbb0a5bd7a0
// https://cdrdv2-public.intel.com/782052/sampling-visible-ggx-normals.pdf

inline Vec3 SampleVNDFHemisphere(Vec3 wo, Vec2 u)
{
    // sample a spherical cap in (-wo.z, 1]
    f64 phi = 2.0 * pi * u.x;
    f64 z = std::fma((1.0 - u.y), (1.0 + wo.z), -wo.z);
    f64 sinTheta = std::sqrt(std::clamp(1.0 - z * z, 0.0, 1.0));
    f64 x = sinTheta * std::cos(phi);
    f64 y = sinTheta * std::sin(phi);
    Vec3 c = Vec3(x, y, z);
    // compute halfway direction;
    Vec3 h = c + wo;

    // return without normalization as this is done later (see line 25)
    return h;
}

inline Vec3 Sample_GGX_VNDF_Dupuy_Benyoub(Vec3 wo, f64 alpha, Vec2 u)
{
    // warp to the hemisphere configuration
    Vec3 woStd = Normalize(Vec3(wo.x * alpha, wo.y * alpha, wo.z));
    // sample the hemisphere
    Vec3 wmStd = SampleVNDFHemisphere(woStd, u);
    // warp back to the ellipsoid configuration
    Vec3 wm = Normalize(Vec3(wmStd.x * alpha, wmStd.y * alpha, wmStd.z));
    // return final normal
    return wm;
}

inline Vec3 Sample_GGX_VNDF_Heitz(Vec3 wo, f64 alpha, Vec2 u)
{
    // Source: "Sampling the GGX Distribution of Visible Normals" by Heitz
    // https://jcgt.org/published/0007/04/01/

    // Section 3.2: transforming the view direction to the hemisphere configuration
    Vec3 Vh{ alpha * wo.x, alpha * wo.y, wo.z };
    Vh.Normalize();

    // Build an orthonormal basis with v, t1, and t2
    // Section 4.1: orthonormal basis (with special case if cross product is zero)
    Vec3 T1 = (Vh.z < 0.999) ? Normalize(Cross(Vh, z_axis)) : x_axis;
    Vec3 T2 = Cross(T1, Vh);

    // Section 4.2: parameterization of the projected area
    f64 r = std::sqrt(u.x);
    f64 phi = two_pi * u.y;
    f64 t1 = r * std::cos(phi);
    f64 t2 = r * std::sin(phi);
    f64 s = 0.5 * (1.0 + Vh.z);
    t2 = Lerp(std::sqrt(1.0 - t1 * t1), t2, s);

    // Section 4.3: reprojection onto hemisphere
    Vec3 Nh = t1 * T1 + t2 * T2 + std::sqrt(std::fmax(0.0, 1.0 - t1 * t1 - t2 * t2)) * Vh;

    // Section 3.4: transforming the normal back to the ellipsoid configuration
    Vec3 h = Normalize(Vec3(alpha * Nh.x, alpha * Nh.y, std::fmax(0.0, Nh.z))); // Sampled half vector

    return h;
}

} // namespace spt
