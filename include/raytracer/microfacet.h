#pragma once

#include "common.h"

namespace spt
{

// https://en.wikipedia.org/wiki/Luma_(video)
inline double Luma(Vec3 color)
{
    return Dot(color, Vec3{ 0.2126, 0.7152, 0.0722 });
    // return Dot(color, Vec3{ 0.299, 0.587, 0.114 });
}

// Default reflectance of dielectrics
constexpr Vec3 default_reflectance{ 0.04 };
constexpr double tolerance = 1e-6;

inline Vec3 F0(Vec3 basecolor, double metallic)
{
    return Lerp(default_reflectance, basecolor, metallic);
}

inline Vec3 F_Schlick(Vec3 f0, double cosine_theta)
{
    return f0 + (Vec3(1.0) - f0) * pow(1.0 - cosine_theta, 5.0);
}

inline double D_GGX(double NoH, double alpha2)
{
    double NoH2 = NoH * NoH;
    double b = (NoH2 * (alpha2 - 1.0) + 1.0);
    return alpha2 / (b * b * pi + tolerance);
}

inline double G1_Smith(double NoV, double alpha2)
{
    double denomC = sqrt(alpha2 + (1.0f - alpha2) * NoV * NoV) + NoV;
    return 2.0f * NoV / (denomC + epsilon);
}

inline double G2_Smith(double NoV, double NoL, double alpha2)
{
    double denomA = NoV * sqrt(alpha2 + (1.0 - alpha2) * NoL * NoL);
    double denomB = NoL * sqrt(alpha2 + (1.0 - alpha2) * NoV * NoV);
    return 2.0 * NoL * NoV / (denomA + denomB + epsilon);
}

// https://google.github.io/filament/Filament.html#materialsystem/specularbrdf/geometricshadowing(specularg)
inline double V_SmithGGXCorrelated(double NoV, double NoL, double alpha2)
{
    double GGXV = NoL * sqrt(NoV * NoV * (1.0 - alpha2) + alpha2);
    double GGXL = NoV * sqrt(NoL * NoL * (1.0 - alpha2) + alpha2);
    return 0.5 / (GGXV + GGXL + epsilon);
}

inline double V_SmithGGXCorrelatedFast(double NoV, double NoL, double alpha)
{
    double GGXV = NoL * (NoV * (1.0 - alpha) + alpha);
    double GGXL = NoV * (NoL * (1.0 - alpha) + alpha);
    return 0.5 / (GGXV + GGXL);
}

} // namespace spt
