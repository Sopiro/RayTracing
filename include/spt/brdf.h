#pragma once

#include "common.h"

namespace spt
{

// Functions for microfacet BRDF

// Default reflectance of dielectrics
constexpr Vec3 default_reflectance{ Float(0.04) };
constexpr Float min_alpha = Float(0.005);

inline Float RoughnessToAlpha(Float roughness)
{
    // return std::fmax(roughness * roughness, min_alpha);
    return std::fmax(roughness, min_alpha);
}

inline Vec3 F0(Vec3 basecolor, Float metallic)
{
    return Lerp(default_reflectance, basecolor, metallic);
}

inline Vec3 F_Schlick(Vec3 f0, Float cosine_theta)
{
    return f0 + (/*f90*/ Vec3(1) - f0) * std::pow(1 - cosine_theta, Float(5.0));
}

// Trowbridge-Reitz distribution
inline Float D_GGX(Float NoH, Float alpha2)
{
    Float b = (NoH * NoH * (alpha2 - 1) + 1);
    return alpha2 * inv_pi / (b * b);
}

inline Float G1_Smith(Float NoV, Float alpha2)
{
    return 2 * NoV / (NoV + std::sqrt(alpha2 + (1 - alpha2) * NoV * NoV));
}

inline Float G2_Smith_Correlated(Float NoV, Float NoL, Float alpha2)
{
    Float g1 = NoV * std::sqrt(alpha2 + (1 - alpha2) * NoL * NoL);
    Float g2 = NoL * std::sqrt(alpha2 + (1 - alpha2) * NoV * NoV);
    return 2 * NoL * NoV / (g1 + g2);
}

inline Float V_Smith_Correlated(Float NoV, Float NoL, Float alpha2)
{
    Float g1 = NoV * std::sqrt(alpha2 + (1 - alpha2) * NoL * NoL);
    Float g2 = NoL * std::sqrt(alpha2 + (1 - alpha2) * NoV * NoV);
    return Float(0.5) / (g1 + g2);
}

// Importance sampling codes

inline Vec3 Sample_GGX(Vec3 wo, Float alpha2, Vec2 u)
{
    Float theta = std::acos(std::sqrt((1 - u.x) / ((alpha2 - 1) * u.x + 1)));
    Float phi = two_pi * u.y;

    Float sin_thetha = std::sin(theta);
    Float x = std::cos(phi) * sin_thetha;
    Float y = std::sin(phi) * sin_thetha;
    Float z = std::cos(theta);

    Vec3 h{ x, y, z }; // Sampled half vector

    return h;
}

// "Sampling Visible GGX Normals with Spherical Caps" by Dupuy & Benyoub
// https://gist.github.com/jdupuy/4c6e782b62c92b9cb3d13fbb0a5bd7a0
// https://cdrdv2-public.intel.com/782052/sampling-visible-ggx-normals.pdf

inline Vec3 SampleVNDFHemisphere(Vec3 wo, Vec2 u)
{
    // sample a spherical cap in (-wo.z, 1]
    Float phi = 2 * pi * u.x;
    Float z = std::fma((1 - u.y), (1 + wo.z), -wo.z);
    Float sinTheta = std::sqrt(std::clamp(1 - z * z, Float(0.0), Float(1.0)));
    Float x = sinTheta * std::cos(phi);
    Float y = sinTheta * std::sin(phi);
    Vec3 c = Vec3(x, y, z);
    // compute halfway direction;
    Vec3 h = c + wo;

    // return without normalization as this is done later (see line 25)
    return h;
}

inline Vec3 Sample_GGX_VNDF_Dupuy_Benyoub(Vec3 wo, Float alpha, Vec2 u)
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

inline Vec3 Sample_GGX_VNDF_Heitz(Vec3 wo, Float alpha, Vec2 u)
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
    Float r = std::sqrt(u.x);
    Float phi = two_pi * u.y;
    Float t1 = r * std::cos(phi);
    Float t2 = r * std::sin(phi);
    Float s = Float(0.5) * (1 + Vh.z);
    t2 = Lerp(std::sqrt(1 - t1 * t1), t2, s);

    // Section 4.3: reprojection onto hemisphere
    Vec3 Nh = t1 * T1 + t2 * T2 + std::sqrt(std::fmax(Float(0.0), 1 - t1 * t1 - t2 * t2)) * Vh;

    // Section 3.4: transforming the normal back to the ellipsoid configuration
    Vec3 h = Normalize(Vec3(alpha * Nh.x, alpha * Nh.y, std::fmax(Float(0.0), Nh.z))); // Sampled half vector

    return h;
}

} // namespace spt
