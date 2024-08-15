#pragma once

#include "common.h"
#include "frame.h"
#include "random.h"

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
    Float f = nf * pdf_f;
    Float g = ng * pdf_g;

    return (f * f) / (f * f + g * g);
}

inline Vec3 SampleUniformHemisphere(const Point2& u)
{
    Float z = u[0];
    Float r = std::sqrt(std::fmax(0.0f, 1 - z * z));
    Float phi = two_pi * u[1];

    return Vec3(r * std::cos(phi), r * std::sin(phi), z);
}

inline Float UniformHemispherePDF()
{
    return inv_two_pi;
}

inline Vec3 SampleUniformSphere(const Point2& u)
{
    Float z = 1 - 2 * u[0];
    Float r = std::sqrt(std::fmax(0.0f, 1 - z * z));
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
inline Vec3 SampleCosineHemisphere(const Point2& u)
{
    Float z = std::sqrt(1 - u[1]);

    Float phi = two_pi * u[0];
    Float su2 = std::sqrt(u[1]);
    Float x = std::cos(phi) * su2;
    Float y = std::sin(phi) * su2;

    return Vec3(x, y, z);
}

inline Float CosineHemispherePDF(Float cos_theta)
{
    return cos_theta * inv_pi;
}

inline Vec3 SampleInsideUnitSphere(const Point2& u)
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
        p = RandVec3(-1.0f, 1.0f);
    } while (Length2(p) >= 1);

    return p;
#endif
}

inline Vec3 SampleUniformUnitDiskXY(const Point2& u)
{
    Float r = std::sqrt(u.x);
    Float theta = two_pi * u.y;
    return Vec3(r * std::cos(theta), r * std::sin(theta), 0);
}

inline Float SampleExponential(Float u, Float a)
{
    return -std::log(1 - u) / a;
}

inline Float ExponentialPDF(Float x, Float a)
{
    return a * std::exp(-a * x);
}

inline Vec3 Sample_GGX(Vec3 wo, Float alpha2, Vec2 u)
{
    BulbitNotUsed(wo);

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
    Float sin_theta = std::sqrt(Clamp(1 - z * z, 0, 1));
    Float x = sin_theta * std::cos(phi);
    Float y = sin_theta * std::sin(phi);
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
    Vec3 T1 = (Vh.z < 0.999f) ? Normalize(Cross(Vh, z_axis)) : x_axis;
    Vec3 T2 = Cross(T1, Vh);

    // Section 4.2: parameterization of the projected area
    Float r = std::sqrt(u.x);
    Float phi = two_pi * u.y;
    Float t1 = r * std::cos(phi);
    Float t2 = r * std::sin(phi);
    Float s = 0.5f * (1 + Vh.z);
    t2 = Lerp(std::sqrt(1 - t1 * t1), t2, s);

    // Section 4.3: reprojection onto hemisphere
    Vec3 Nh = t1 * T1 + t2 * T2 + std::sqrt(std::fmax(0.0f, 1 - t1 * t1 - t2 * t2)) * Vh;

    // Section 3.4: transforming the normal back to the ellipsoid configuration
    Vec3 h = Normalize(Vec3(alpha_x * Nh.x, alpha_y * Nh.y, std::fmax(0.0f, Nh.z))); // Sampled half vector

    return h;
}

inline Float HenyeyGreenstein(Float cos_theta, Float g)
{
    Float denom = 1 + Sqr(g) + 2 * g * cos_theta;
    return inv_four_pi * (1 - Sqr(g)) / (denom * SafeSqrt(denom));
}

inline Vec3 SampleHenyeyGreenstein(Vec3 wo, Float g, Point2 u, Float* pdf)
{
    g = Clamp(g, -1 + epsilon, 1 - epsilon);

    Float cos_theta;
    if (std::abs(g) < 1e-3f)
    {
        cos_theta = 1 - 2 * u[0];
    }
    else
    {
        cos_theta = -1 / (2 * g) * (1 + Sqr(g) - Sqr((1 - Sqr(g)) / (1 + g - 2 * g * u[0])));
    }

    // Compute direction wi with respect to the default scattering coordinate
    Float sin_theta = SafeSqrt(1 - Sqr(cos_theta));
    Float phi = two_pi * u[1];
    Vec3 wi_local = SphericalDirection(sin_theta, cos_theta, phi);

    // Return back to the wo’s coordinate system
    Frame frame = Frame::FromZ(wo);
    Vec3 wi = frame.FromLocal(wi_local);

    if (pdf)
    {
        *pdf = HenyeyGreenstein(cos_theta, g);
    }

    return wi;
}

inline int32 SampleDiscrete(std::span<const Float> weights, Float u, Float* pmf = nullptr, Float* u_remapped = nullptr)
{
    if (weights.empty())
    {
        if (pmf)
        {
            *pmf = 0;
        }
        return -1;
    }

    Float sum_weights = 0;
    for (Float w : weights)
    {
        sum_weights += w;
    }

    Float up = u * sum_weights;
    if (up == sum_weights)
    {
        up -= epsilon;
    }

    int32 offset = 0;
    Float sum = 0;
    while (sum + weights[offset] <= up)
    {
        sum += weights[offset++];
    }

    if (pmf)
    {
        *pmf = weights[offset] / sum_weights;
    }
    if (u_remapped)
    {
        *u_remapped = std::min<Float>((up - sum) / weights[offset], Float(1) - epsilon);
    }

    return offset;
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

// https://sopiro.github.io/posts/wrs/
// https://www.pbr-book.org/4ed/Sampling_Algorithms/Reservoir_Sampling
template <typename T>
class WeightedReservoirSampler
{
public:
    WeightedReservoirSampler(uint64 seed)
        : rng(seed)
        , reservoir_weight{ 0 }
        , weight_sum{ 0 }
    {
    }

    void Seed(uint64 seed)
    {
        rng.Seed(seed);
    }

    bool Add(const T& sample, Float weight)
    {
        weight_sum += weight;

        if (rng.NextFloat() < weight / weight_sum)
        {
            reservoir = sample;
            reservoir_weight = weight;
            return true;
        }

        return false;
    }

    bool HasSample() const
    {
        return weight_sum > 0;
    }

    const T& GetSample() const
    {
        return reservoir;
    }

    Float GetSampleProbability() const
    {
        return reservoir_weight / weight_sum;
    }

    Float GetWeightSum() const
    {
        return weight_sum;
    }

    Float Reset()
    {
        reservoir_weight = 0;
        weight_sum = 0;
    }

    bool Merge(const WeightedReservoirSampler& other)
    {
        if (other.HasSample() && Add(other.reservoir, other.weight_sum))
        {
            reservoir_weight = other.reservoir_weight;
            return true;
        }

        return false;
    }

private:
    RNG rng;

    T reservoir{};
    Float reservoir_weight;
    Float weight_sum;
};

} // namespace bulbit
