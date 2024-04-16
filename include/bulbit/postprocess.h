#pragma once

#include "spectrum.h"

namespace bulbit
{

// https://en.wikipedia.org/wiki/SRGB
inline Spectrum ToLinearRGB(const Spectrum& color)
{
    Spectrum result;

    for (int i = 0; i < 3; ++i)
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

inline Spectrum ToSRGB(const Spectrum& color)
{
    Spectrum result;

    for (int i = 0; i < 3; ++i)
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
inline Spectrum Tonemap_Reinhard(const Spectrum& hdr)
{
    return hdr / (Spectrum(1.0) + hdr);
}

inline Spectrum Tonemap_Reinhard2(const Spectrum& hdr)
{
    const Spectrum L_white{ 4 };
    return (hdr * (Spectrum(1.0) + hdr / (L_white * L_white))) / (Spectrum(1.0) + hdr);
}

inline Spectrum Tonemap_ACES(const Spectrum& hdr)
{
    constexpr Float a = Float(2.51);
    constexpr Float b = Float(0.03);
    constexpr Float c = Float(2.43);
    constexpr Float d = Float(0.59);
    constexpr Float e = Float(0.14);
    return (hdr * (a * hdr + Spectrum(b))) / (hdr * (c * hdr + Spectrum(d)) + Spectrum(e));
}

inline Spectrum Tonemap_Unreal(const Spectrum& hdr)
{
    // Unreal 3, Documentation: "Spectrum Grading"
    // Adapted to be close to Tonemap_ACES, with similar range
    // Gamma 2.2 correction is baked in, don't use with sRGB conversion!
    return hdr / (hdr + Spectrum(Float(0.155))) * Float(1.019);
}

// https://www.shadertoy.com/view/llsSD2
inline Spectrum Tonemap_FilmicHejl2015(const Spectrum& hdr, Float whitePt)
{
    Vec4 vh = Vec4(hdr.r, hdr.g, hdr.b, whitePt);
    Vec4 va = Float(1.425) * vh + Float(0.05); // eval filmic curve
    Vec4 vf = (vh * va + Float(0.004)) / (vh * (va + Float(0.55)) + Float(0.0491)) - Float(0.0821);
    return Spectrum(vf.x / vf.w, vf.y / vf.w, vf.z / vf.w); // white point correction
}

} // namespace bulbit
