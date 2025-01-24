#pragma once

#include "math.h"

namespace bulbit
{

// https://en.wikipedia.org/wiki/SRGB
inline Vec3 RGB_from_sRGB(const Vec3& sRGB)
{
    Vec3 RGB;

    for (int32 i = 0; i < 3; ++i)
    {
        Float comp = sRGB[i];

        if (comp <= 0.04045f)
        {
            RGB[i] = comp * (1.0f / 12.92f);
        }
        else
        {
            RGB[i] = std::pow((comp + 0.055f) * (1.0f / 1.055f), 2.4f);
        }
    }

    return RGB;
}

inline Vec3 sRGB_from_RGB(const Vec3& RGB)
{
    Vec3 sRGB;

    for (int32 i = 0; i < 3; ++i)
    {
        Float comp = RGB[i];

        if (comp <= 0.0031308f)
        {
            sRGB[i] = 12.92f * comp;
        }
        else
        {
            sRGB[i] = (1.0f + 0.055f) * std::pow(comp, 1.0f / 2.4f) - 0.055f;
        }
    }

    return sRGB;
}

// Tone mapping functions

// https://www.shadertoy.com/view/WdjSW3
inline Vec3 Tonemap_Reinhard(const Vec3& RGB)
{
    return RGB / (Vec3(1.0) + RGB);
}

inline Vec3 Tonemap_Reinhard2(const Vec3& RGB)
{
    const Vec3 L_white{ 4 };
    return (RGB * (Vec3(1.0) + RGB / (L_white * L_white))) / (Vec3(1.0) + RGB);
}

inline Vec3 Tonemap_ACES(const Vec3& RGB)
{
    const Float a = 2.51f;
    const Float b = 0.03f;
    const Float c = 2.43f;
    const Float d = 0.59f;
    const Float e = 0.14f;
    return (RGB * (a * RGB + Vec3(b))) / (RGB * (c * RGB + Vec3(d)) + Vec3(e));
}

} // namespace bulbit
