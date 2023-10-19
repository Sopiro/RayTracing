#pragma once

#include "random.h"

namespace bulbit
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
    Float r = std::sqrt(std::fmax(Float(0.0), 1 - z * z));
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

struct Distribution1D
{
    Distribution1D(const Float* f, int32 n)
        : func(f, f + n)
        , cdf(n + 1)
    {
        cdf[0] = 0;
        for (int32 i = 1; i < n + 1; ++i)
        {
            cdf[i] = cdf[i - 1] + func[i - 1] / n;
        }

        func_integral = cdf[n];
        if (func_integral == 0)
        {
            for (int32 i = 1; i < n + 1; ++i)
            {
                cdf[i] = Float(i) / Float(n);
            }
        }
        else
        {
            // Normalization
            for (int32 i = 1; i < n + 1; ++i)
            {
                cdf[i] /= func_integral;
            }
        }
    }

    Float SampleContinuous(Float* pdf, Float u, int32* off = nullptr) const
    {
        // Find the starting offset of cdf
        int32 offset = FindInterval((int32)cdf.size(), [&](int32 index) { return cdf[index] <= u; });

        if (off)
        {
            *off = offset;
        }

        if (pdf)
        {
            *pdf = func[offset] / func_integral;
        }

        Float du = (u - cdf[offset]) / (cdf[offset + 1] - cdf[offset]);

        return (offset + du) / Count();
    }

    int32 SampleDiscrete(Float u, Float* pdf = nullptr, Float* u_remapped = nullptr) const
    {
        int32 offset = FindInterval((int32)cdf.size(), [&](int32 index) { return cdf[index] <= u; });

        if (pdf)
        {
            *pdf = func[offset] / (func_integral * Count());
        }

        if (u_remapped)
        {
            *u_remapped = (u - cdf[offset]) / (cdf[offset + 1] - cdf[offset]);
        }

        return offset;
    }

    int32 Count() const
    {
        return (int32)func.size();
    }

    Float DiscretePDF(int32 index) const
    {
        return func[index] / (func_integral * Count());
    }

    std::vector<Float> func, cdf;
    Float func_integral;
};

class Distribution2D
{
public:
    Distribution2D(const Float* func, int32 nu, int32 nv)
    {
        conditional_v.reserve(nv);
        for (int32 v = 0; v < nv; ++v)
        {
            conditional_v.emplace_back(new Distribution1D(&func[v * nu], nu));
        }

        std::vector<Float> marginal_func(nv);
        for (int32 v = 0; v < nv; ++v)
        {
            marginal_func[v] = conditional_v[v]->func_integral;
        }

        marginal.reset(new Distribution1D(&marginal_func[0], nv));
    }

    Point2 SampleContinuous(Float* pdf, const Point2& u) const
    {
        Float pdfs[2];
        int32 v;

        Float d1 = marginal->SampleContinuous(&pdfs[1], u[1], &v);
        Float d0 = conditional_v[v]->SampleContinuous(&pdfs[0], u[0]);

        *pdf = pdfs[0] * pdfs[1];
        return Point2(d0, d1);
    }

    Float Pdf(const Point2& p) const
    {
        int32 w = conditional_v[0]->Count();
        int32 h = marginal->Count();

        int32 iu = Clamp(int32(p[0] * w), 0, w - 1);
        int32 iv = Clamp(int32(p[1] * h), 0, h - 1);

        return conditional_v[iv]->func[iu] / marginal->func_integral;
    }

private:
    std::vector<std::unique_ptr<Distribution1D>> conditional_v; // p(u|v)
    std::unique_ptr<Distribution1D> marginal;
};

} // namespace bulbit
