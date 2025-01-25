#pragma once

#include "math.h"

namespace bulbit
{

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

// See below for more:
// https://www.khronos.org/news/press/khronos-pbr-neutral-tone-mapper-released-for-true-to-life-color-rendering-of-3d-products
inline Vec3 ToneMap_PBRNeutral(Vec3 RGB)
{
    const Float startCompression = 0.8f - 0.04f;
    const Float desaturation = 0.15f;

    Float x = std::min(RGB.x, std::min(RGB.y, RGB.z));
    Float offset = x < 0.08f ? x - 6.25f * x * x : 0.04f;
    RGB -= Vec3(offset);

    Float peak = std::max(RGB.x, std::max(RGB.y, RGB.z));
    if (peak < startCompression) return RGB;

    const Float d = 1.f - startCompression;
    Float newPeak = 1.f - d * d / (peak + d - startCompression);
    RGB *= newPeak / peak;

    Float g = 1.f - 1.f / (desaturation * (peak - newPeak) + 1.f);
    return Lerp(RGB, newPeak * Vec3(1, 1, 1), g);
}

} // namespace bulbit
