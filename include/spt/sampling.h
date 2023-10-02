#pragma once

#include "random.h"

namespace spt
{

inline Vec3 UniformSampleHemisphere()
{
    Float u1 = Rand();
    Float u2 = Rand();

    Float z = u1;
    Float r = std::sqrt(std::fmax(Float(0.0), Float(1.0) - z * z));
    Float phi = two_pi * u2;

    return Vec3(r * std::cos(phi), r * std::sin(phi), z);
}

inline Vec3 UniformSampleSphere()
{
    Float u1 = Rand();
    Float u2 = Rand();

    Float z = 1 - 2 * u1;
    Float r = std::sqrt(std::fmax(0, 1 - z * z));
    Float phi = two_pi * u2;

    Float x = r * std::cos(phi);
    Float y = r * std::sin(phi);

    return Vec3(x, y, z);
}

inline Float UniformSampleSpherePDF()
{
    return inv_four_pi;
}

// z > 0
inline Vec3 CosineSampleHemisphere()
{
    Float u1 = Rand();
    Float u2 = Rand();

    Float z = std::sqrt(Float(1.0) - u2);

    Float phi = two_pi * u1;
    Float su2 = std::sqrt(u2);
    Float x = std::cos(phi) * su2;
    Float y = std::sin(phi) * su2;

    return Vec3(x, y, z);
}

inline Float CosineSampleHemispherePDF(Float cos_theta)
{
    return cos_theta * inv_pi;
}

inline Vec3 RandomInUnitSphere()
{
#if 0
    Float u1 = Rand();
    Float u2 = Rand();

    Float theta = two_pi * u1;
    Float phi = std::acos(2.0 * u2 - 1.0);

    Float sin_phi = std::sin(phi);

    Float x = sin_phi * std::cos(theta);
    Float y = sin_phi * std::sin(theta);
    Float z = std::cos(phi);

    return Vec3(x, y, z);
#else
    // Rejection sampling
    Vec3 p;
    do
    {
        p = RandVec3(Float(-1.0), Float(1.0));
    }
    while (p.Length2() >= Float(1.0));

    return p;
#endif
}

inline Vec3 UniformSampleUnitDiskXY()
{
    Float u1 = Rand();
    Float u2 = Rand();

    Float r = std::sqrt(u1);
    Float theta = two_pi * u2;
    return Vec3(r * std::cos(theta), r * std::sin(theta), Float(0.0));
}

// Heuristic functions for MIS
inline Float BalanceHeuristic(Float pdf_f, Float pdf_g)
{
    return pdf_f / (pdf_f + pdf_g);
}

inline Float BalanceHeuristic(int32 nf, Float pdf_f, int32 ng, Float pdf_g)
{
    return (nf * pdf_f) / (nf * pdf_f + ng * pdf_g);
}

inline Float PowerHeuristic(Float pdf_f, Float pdf_g)
{
    Float f2 = pdf_f * pdf_f;
    Float g2 = pdf_g * pdf_g;
    return f2 / (f2 + g2);
}

inline Float PowerHeuristic(int32 nf, Float pdf_f, int32 ng, Float pdf_g)
{
    Float f = nf * pdf_f, g = ng * pdf_g;
    return (f * f) / (f * f + g * g);
}

} // namespace spt
