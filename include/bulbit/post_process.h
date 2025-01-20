#pragma once

#include "math.h"

namespace bulbit
{

// https://en.wikipedia.org/wiki/SRGB
inline Vec3 ToLinearRGB(const Vec3& color)
{
    Vec3 result;

    for (int32 i = 0; i < 3; ++i)
    {
        Float comp = color[i];

        if (comp <= 0.04045f)
        {
            result[i] = comp * (1.0f / 12.92f);
        }
        else
        {
            result[i] = std::pow((comp + 0.055f) * (1.0f / 1.055f), 2.4f);
        }
    }

    return result;
}

inline Vec3 ToSRGB(const Vec3& color)
{
    Vec3 result;

    for (int32 i = 0; i < 3; ++i)
    {
        Float comp = color[i];

        if (comp <= 0.0031308f)
        {
            result[i] = 12.92f * comp;
        }
        else
        {
            result[i] = (1.0f + 0.055f) * std::pow(comp, 1.0f / 2.4f) - 0.055f;
        }
    }

    return result;
}

// Tone mapping functions

// https://www.shadertoy.com/view/WdjSW3
inline Vec3 Tonemap_Reinhard(const Vec3& hdr)
{
    return hdr / (Vec3(1.0) + hdr);
}

inline Vec3 Tonemap_Reinhard2(const Vec3& hdr)
{
    const Vec3 L_white{ 4 };
    return (hdr * (Vec3(1.0) + hdr / (L_white * L_white))) / (Vec3(1.0) + hdr);
}

inline Vec3 Tonemap_ACES(const Vec3& hdr)
{
    constexpr Float a = 2.51f;
    constexpr Float b = 0.03f;
    constexpr Float c = 2.43f;
    constexpr Float d = 0.59f;
    constexpr Float e = 0.14f;
    return (hdr * (a * hdr + Vec3(b))) / (hdr * (c * hdr + Vec3(d)) + Vec3(e));
}

inline Vec3 Tonemap_Unreal(const Vec3& hdr)
{
    // Unreal 3, Documentation: "Spectrum Grading"
    // Adapted to be close to Tonemap_ACES, with similar range
    // Gamma 2.2 correction is baked in, don't use with sRGB conversion!
    return hdr / (hdr + Vec3(0.155f)) * 1.019f;
}

// https://www.shadertoy.com/view/llsSD2
inline Vec3 Tonemap_FilmicHejl2015(const Vec3& hdr, Float whitePt)
{
    Vec4 vh = Vec4(hdr.x, hdr.y, hdr.z, whitePt);
    Vec4 va = Float(1.425) * vh + Float(0.05);          // eval filmic curve
    Vec4 vf = (vh * va + Float(0.004)) / (vh * (va + Float(0.55)) + Float(0.0491)) - Float(0.0821);
    return Vec3(vf.x / vf.w, vf.y / vf.w, vf.z / vf.w); // white point correction
}

} // namespace bulbit
