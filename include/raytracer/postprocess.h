#pragma once

#include "common.h"

namespace spt
{

inline Vec3 GammaCorrection(const Color& color, double gamma)
{
    double invGamma = 1.0 / gamma;
    return Vec3{ pow(color.x, invGamma), pow(color.y, invGamma), pow(color.z, invGamma) };
}

// https://www.shadertoy.com/view/WdjSW3

inline Vec3 Reinhard(const Vec3& hdr)
{
    return hdr / (Vec3{ 1.0 } + hdr);
}

inline Vec3 Reinhard2(const Vec3& hdr)
{
    constexpr Vec3 L_white{ 4.0 };
    return (hdr * (Vec3{ 1.0 } + hdr / (L_white * L_white))) / (Vec3{ 1.0 } + hdr);
}

inline Vec3 Tonemap_ACES(const Vec3& hdr)
{
    constexpr double a = 2.51;
    constexpr double b = 0.03;
    constexpr double c = 2.43;
    constexpr double d = 0.59;
    constexpr double e = 0.14;
    return (hdr * (a * hdr + Vec3{ b })) / (hdr * (c * hdr + Vec3{ d }) + Vec3{ e });
}

inline Vec3 Tonemap_Unreal(const Vec3& hdr)
{
    // Unreal 3, Documentation: "Color Grading"
    // Adapted to be close to Tonemap_ACES, with similar range
    // Gamma 2.2 correction is baked in, don't use with sRGB conversion!
    return hdr / (hdr + Vec3{ 0.155 }) * 1.019;
}

// https://www.shadertoy.com/view/llsSD2
inline Vec3 ToneMapFilmic_Hejl2015(const Vec3& hdr, double whitePt)
{
    Vec4 vh = Vec4(hdr, whitePt);
    Vec4 va = 1.425 * vh + 0.05; // eval filmic curve
    Vec4 vf = (vh * va + 0.004) / (vh * (va + 0.55) + 0.0491) - 0.0821;
    return Vec3{ vf.x / vf.w, vf.y / vf.w, vf.z / vf.w }; // white point correction
}

} // namespace spt
