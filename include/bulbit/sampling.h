#pragma once

#include "common.h"

namespace bulbit
{

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

inline Vec3 UniformSampleHemisphere(const Point2& u)
{
    Float z = u[0];
    Float r = std::sqrt(std::fmax(Float(0.0), 1 - z * z));
    Float phi = two_pi * u[1];

    return Vec3(r * std::cos(phi), r * std::sin(phi), z);
}

inline Float UniformHemispherePDF()
{
    return inv_two_pi;
}

inline Vec3 UniformSampleSphere(const Point2& u)
{
    Float z = 1 - 2 * u[0];
    Float r = std::sqrt(std::fmax(Float(0.0), 1 - z * z));
    Float phi = two_pi * u[1];

    Float x = r * std::cos(phi);
    Float y = r * std::sin(phi);

    return Vec3(x, y, z);
}

inline Float UniformSampleSpherePDF()
{
    return inv_four_pi;
}

// z > 0
inline Vec3 CosineSampleHemisphere(const Point2& u)
{
    Float z = std::sqrt(1 - u[1]);

    Float phi = two_pi * u[0];
    Float su2 = std::sqrt(u[1]);
    Float x = std::cos(phi) * su2;
    Float y = std::sin(phi) * su2;

    return Vec3(x, y, z);
}

inline Float CosineSampleHemispherePDF(Float cos_theta)
{
    return cos_theta * inv_pi;
}

inline Vec3 RandomInUnitSphere(const Point2& u)
{
#if 1
    Float theta = two_pi * u[0];
    Float phi = std::acos(2 * u[1] - 1);

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
        p = RandVec3(Float(-1.0), 1);
    }
    while (p.Length2() >= 1);

    return p;
#endif
}

inline Vec3 UniformSampleUnitDiskXY(const Point2& u)
{
    Float r = std::sqrt(u.x);
    Float theta = two_pi * u.y;
    return Vec3(r * std::cos(theta), r * std::sin(theta), Float(0.0));
}

// Importance sampling codes for microfacet functions

inline Vec3 Sample_GGX(Vec3 wo, Float alpha2, Vec2 u)
{
    Float theta = std::acos(std::sqrt((1 - u.x) / ((alpha2 - 1) * u.x + 1)));
    Float phi = two_pi * u.y;

    Float sin_thetha = std::sin(theta);
    Float x = std::cos(phi) * sin_thetha;
    Float y = std::sin(phi) * sin_thetha;
    Float z = std::cos(theta);

    Vec3 h{ x, y, z }; // Sampled half vector

    return h;
}

// "Sampling Visible GGX Normals with Spherical Caps" by Dupuy & Benyoub
// https://gist.github.com/jdupuy/4c6e782b62c92b9cb3d13fbb0a5bd7a0
// https://cdrdv2-public.intel.com/782052/sampling-visible-ggx-normals.pdf
inline Vec3 SampleVNDFHemisphere(Vec3 wo, Vec2 u)
{
    // sample a spherical cap in (-wo.z, 1]
    Float phi = two_pi * u.x;
    Float z = std::fma((1 - u.y), (1 + wo.z), -wo.z);
    Float sinTheta = std::sqrt(Clamp(1 - z * z, 0, 1));
    Float x = sinTheta * std::cos(phi);
    Float y = sinTheta * std::sin(phi);
    Vec3 c = Vec3(x, y, z);
    // compute halfway direction;
    Vec3 h = c + wo;

    // return without normalization as this is done later (see line 25)
    return h;
}

inline Vec3 Sample_GGX_VNDF_Dupuy_Benyoub(Vec3 wo, Float alpha_x, Float alpha_y, Vec2 u)
{
    // warp to the hemisphere configuration
    Vec3 woStd = Normalize(Vec3(wo.x * alpha_x, wo.y * alpha_y, wo.z));
    // sample the hemisphere
    Vec3 wmStd = SampleVNDFHemisphere(woStd, u);
    // warp back to the ellipsoid configuration
    Vec3 wm = Normalize(Vec3(wmStd.x * alpha_x, wmStd.y * alpha_y, wmStd.z));
    // return final normal
    return wm;
}

// Source: "Sampling the GGX Distribution of Visible Normals" by Heitz
// https://jcgt.org/published/0007/04/01/
inline Vec3 Sample_GGX_VNDF_Heitz(Vec3 wo, Float alpha_x, Float alpha_y, Vec2 u)
{
    // Section 3.2: transforming the view direction to the hemisphere configuration
    Vec3 Vh{ alpha_x * wo.x, alpha_y * wo.y, wo.z };
    Vh.Normalize();

    // Build an orthonormal basis with v, t1, and t2
    // Section 4.1: orthonormal basis (with special case if cross product is zero)
    Vec3 T1 = (Vh.z < Float(0.999)) ? Normalize(Cross(Vh, z_axis)) : x_axis;
    Vec3 T2 = Cross(T1, Vh);

    // Section 4.2: parameterization of the projected area
    Float r = std::sqrt(u.x);
    Float phi = two_pi * u.y;
    Float t1 = r * std::cos(phi);
    Float t2 = r * std::sin(phi);
    Float s = Float(0.5) * (1 + Vh.z);
    t2 = Lerp(std::sqrt(1 - t1 * t1), t2, s);

    // Section 4.3: reprojection onto hemisphere
    Vec3 Nh = t1 * T1 + t2 * T2 + std::sqrt(std::fmax(Float(0), 1 - t1 * t1 - t2 * t2)) * Vh;

    // Section 3.4: transforming the normal back to the ellipsoid configuration
    Vec3 h = Normalize(Vec3(alpha_x * Nh.x, alpha_y * Nh.y, std::fmax(Float(0), Nh.z))); // Sampled half vector

    return h;
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
