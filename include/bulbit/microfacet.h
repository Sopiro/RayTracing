#pragma once

#include "common.h"
#include "frame.h"
#include "sampling.h"
#include "spectrum.h"

namespace bulbit
{

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
        return std::max(alpha_x, alpha_y) < 1e-3f;
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

    Vec3 Sample_Wm(const Vec3& w, const Point2& u) const
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

    void Regularize()
    {
        if (alpha_x < 0.3f) alpha_x = Clamp(2 * alpha_x, 0.1f, 0.3f);
        if (alpha_y < 0.3f) alpha_y = Clamp(2 * alpha_y, 0.1f, 0.3f);
    }

private:
    Float alpha_x, alpha_y;
};

} // namespace bulbit
