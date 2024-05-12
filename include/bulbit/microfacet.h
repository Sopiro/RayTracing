#pragma once

#include "common.h"
#include "sampling.h"
#include "spectrum.h"

namespace bulbit
{

// Functions for microfacet BRDF

// Default reflectance of dielectrics
constexpr Vec3 default_reflectance(Float(0.04));
constexpr Float min_alpha = Float(0.002);

inline Float RoughnessToAlpha(Float roughness)
{
    return std::fmax(roughness * roughness, min_alpha);
}

inline Spectrum F0(Spectrum basecolor, Float metallic)
{
    return Lerp(default_reflectance, basecolor, metallic);
}

inline Spectrum F_Schlick(Spectrum f0, Float cosine_theta)
{
    return f0 + (/*f90*/ Spectrum(1) - f0) * std::pow(1 - cosine_theta, Float(5.0));
}

// Trowbridge-Reitz distribution
inline Float D_GGX(Float NoH, Float alpha2)
{
    Float b = (NoH * NoH * (alpha2 - 1) + 1);
    return alpha2 * inv_pi / (b * b + Float(1e-7));
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

class TrowbridgeReitzDistribution
{
public:
    TrowbridgeReitzDistribution(Float alpha_x, Float alpha_y)
        : alpha_x{ alpha_x }
        , alpha_y{ alpha_y }
    {
        if (!EffectivelySmooth())
        {
            alpha_x = std::max(alpha_x, Float(1e-4));
            alpha_y = std::max(alpha_y, Float(1e-4));
        }
    }

    bool EffectivelySmooth() const
    {
        return std::max(alpha_x, alpha_y) < Float(1e-3);
    }

    Float D(const Vec3& wm) const
    {
        Float tan2_theta = Tan2Theta(wm);
        if (std::isinf(tan2_theta))
        {
            return 0;
        }

        Float cos4_theta = Sqr(Cos2Theta(wm));

        if (cos4_theta < 1e-16f)
        {
            return 0;
        }
        Float e = tan2_theta * (Sqr(CosPhi(wm) / alpha_x) + Sqr(SinPhi(wm) / alpha_y));

        return 1 / (pi * alpha_x * alpha_y * cos4_theta * Sqr(1 + e));
    }

    Float D(const Vec3& w, const Vec3& wm) const
    {
        return G1(w) / AbsCosTheta(w) * D(wm) * AbsDot(w, wm);
    }

    Float PDF(const Vec3& w, const Vec3& wm) const
    {
        return D(w, wm);
    }

    Float G1(const Vec3& w) const
    {
        return 1 / (1 + Lambda(w));
    }

    Float G(const Vec3& wo, const Vec3& wi) const
    {
        return 1 / (1 + Lambda(wo) + Lambda(wi));
    }

    Float Lambda(const Vec3& w) const
    {
        Float tan2_theta = Tan2Theta(w);
        if (std::isinf(tan2_theta))
        {
            return 0;
        }

        Float alpha2 = Sqr(CosPhi(w) * alpha_x) + Sqr(SinPhi(w) * alpha_y);
        return (std::sqrt(1 + alpha2 * tan2_theta) - 1) / 2;
    }

    Vec3 SampleWm(const Vec3& w, const Point2& u) const
    {
#if 1
        Vec3 wm = Sample_GGX_VNDF_Dupuy_Benyoub(w, alpha_x, alpha_y, u);
#else
        Vec3 wm = Sample_GGX_VNDF_Heitz(w, alpha_x, alpha_y, u);
#endif
        return wm;
    }

    static Float RoughnessToAlpha(Float roughness)
    {
        return std::sqrt(roughness);
    }

private:
    Float alpha_x, alpha_y;
};

} // namespace bulbit
