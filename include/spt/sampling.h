#pragma once

#include "random.h"

namespace spt
{

inline Vec3 UniformSampleHemisphere()
{
    Real u1 = Rand();
    Real u2 = Rand();

    f64 z = u1;
    f64 r = std::sqrt(std::max((f64)0.0, (f64)1.0 - z * z));
    f64 phi = 2.0 * pi * u2;

    return Vec3(r * std::cos(phi), r * std::sin(phi), z);
}

inline Vec3 UniformSampleSphere()
{
    Real u1 = Rand();
    Real u2 = Rand();

    Real z = 1.0 - 2.0 * u1;
    Real r = std::sqrt(std::fmax(0.0, 1.0 - z * z));
    Real phi = two_pi * u2;

    Real x = r * std::cos(phi);
    Real y = r * std::sin(phi);

    return Vec3(x, y, z);
}

// z > 0
inline Vec3 CosineSampleHemisphere()
{
    Real u1 = Rand();
    Real u2 = Rand();

    Real z = std::sqrt(Real(1.0) - u2);

    Real phi = two_pi * u1;
    Real su2 = std::sqrt(u2);
    Real x = std::cos(phi) * su2;
    Real y = std::sin(phi) * su2;

    return Vec3(x, y, z);
}

inline Vec3 RandomInUnitSphere()
{
#if 0
    Real u1 = Rand();
    Real u2 = Rand();

    Real theta = two_pi * u1;
    Real phi = std::acos(2.0 * u2 - 1.0);

    Real sin_phi = std::sin(phi);

    Real x = sin_phi * std::cos(theta);
    Real y = sin_phi * std::sin(theta);
    Real z = std::cos(phi);

    return Vec3(x, y, z);
#else
    // Rejection sampling
    Vec3 p;
    do
    {
        p = RandVec3(Real(-1.0), Real(1.0));
    }
    while (p.Length2() >= Real(1.0));

    return p;
#endif
}

inline Vec3 UniformSampleUnitDiskXY()
{
    Real u1 = Rand();
    Real u2 = Rand();

    Real r = std::sqrt(u1);
    Real theta = two_pi * u2;
    return Vec3(r * std::cos(theta), r * std::sin(theta), Real(0.0));
}

// Heuristic functions for MIS
inline f64 BalanceHeuristic(f64 pdf_f, f64 pdf_g)
{
    return pdf_f / (pdf_f + pdf_g);
}

inline f64 BalanceHeuristic(i32 nf, f64 pdf_f, i32 ng, f64 pdf_g)
{
    return (nf * pdf_f) / (nf * pdf_f + ng * pdf_g);
}

inline f64 PowerHeuristic(f64 pdf_f, f64 pdf_g)
{
    f64 f2 = pdf_f * pdf_f;
    f64 g2 = pdf_g * pdf_g;
    return f2 / (f2 + g2);
}

inline f64 PowerHeuristic(i32 nf, f64 pdf_f, i32 ng, f64 pdf_g)
{
    f64 f = nf * pdf_f, g = ng * pdf_g;
    return (f * f) / (f * f + g * g);
}

} // namespace spt
