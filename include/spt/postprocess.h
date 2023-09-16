#pragma once

#include "common.h"

namespace spt
{

inline Color ToLinear(const Color& color, f64 gamma = 2.2)
{
    return Color{ std::pow(color.x, gamma), std::pow(color.y, gamma), std::pow(color.z, gamma) };
}

inline Color GammaCorrection(const Color& color, f64 gamma = 2.2)
{
    f64 invGamma = 1.0 / gamma;
    return Color{ std::pow(color.x, invGamma), std::pow(color.y, invGamma), std::pow(color.z, invGamma) };
}

// Tone mapping functions

// https://www.shadertoy.com/view/WdjSW3
inline Color Tonemap_Reinhard(const Color& hdr)
{
    return hdr / (Color{ 1.0 } + hdr);
}

inline Color Tonemap_Reinhard2(const Color& hdr)
{
    constexpr Color L_white{ 4.0 };
    return (hdr * (Color{ 1.0 } + hdr / (L_white * L_white))) / (Color{ 1.0 } + hdr);
}

inline Color Tonemap_ACES(const Color& hdr)
{
    constexpr f64 a = 2.51;
    constexpr f64 b = 0.03;
    constexpr f64 c = 2.43;
    constexpr f64 d = 0.59;
    constexpr f64 e = 0.14;
    return (hdr * (a * hdr + Color{ b })) / (hdr * (c * hdr + Color{ d }) + Color{ e });
}

inline Color Tonemap_Unreal(const Color& hdr)
{
    // Unreal 3, Documentation: "Color Grading"
    // Adapted to be close to Tonemap_ACES, with similar range
    // Gamma 2.2 correction is baked in, don't use with sRGB conversion!
    return hdr / (hdr + Color{ 0.155 }) * 1.019;
}

// https://www.shadertoy.com/view/llsSD2
inline Color Tonemap_FilmicHejl2015(const Color& hdr, f64 whitePt)
{
    Vec4 vh = Vec4(hdr, whitePt);
    Vec4 va = 1.425 * vh + 0.05; // eval filmic curve
    Vec4 vf = (vh * va + 0.004) / (vh * (va + 0.55) + 0.0491) - 0.0821;
    return Color{ vf.x / vf.w, vf.y / vf.w, vf.z / vf.w }; // white point correction
}

} // namespace spt
