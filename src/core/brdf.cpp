#include "bulbit/brdf.h"
#include "bulbit/microfacet.h"
#include "bulbit/sampling.h"

namespace bulbit
{

LambertianReflection::LambertianReflection(const Vec3& n)
    : frame{ n }
{
}

Vec3 LambertianReflection::Sample(const Point2& u) const
{
    return frame.FromLocal(CosineSampleHemisphere(u));
}

Float LambertianReflection::Evaluate(const Vec3& wi) const
{
    Float cosine = Dot(wi, frame.z);
    return cosine <= 0 ? 0 : cosine * inv_pi;
}

MicrofacetGGX::MicrofacetGGX(const Vec3& n, const Vec3& wo, Float a, Float t)
    : frame{ n }
    , wo{ wo }
    , alpha2{ a * a }
    , t{ t }
{
}

Vec3 MicrofacetGGX::Sample(const Point2& u0) const
{
    Point2 u = u0;
    if (u[0] < t)
    {
        u[0] /= t;

        Vec3 w = frame.ToLocal(wo);
        Vec3 h = Sample_GGX(w, alpha2, u);
        Vec3 wh = frame.FromLocal(h);
        Vec3 wi = Reflect(wo, wh);

        return wi;
    }
    else
    {
        u[0] = (u[0] - t) / (1 - t);

        Vec3 random_cosine = CosineSampleHemisphere(u);
        return frame.FromLocal(random_cosine);
    }
}

Float MicrofacetGGX::Evaluate(const Vec3& wi) const
{
    Vec3 h = Normalize(wo + wi);
    Float NoH = Dot(h, frame.z);
    Float LoH = Dot(/*L*/ wi, /*H*/ frame.z);
    Float spec_w = D_GGX(NoH, alpha2) * NoH / std::fmax(4 * LoH, Float(0.0));

    Float diff_w = LoH * inv_pi;

    return (1 - t) * diff_w + t * spec_w;
}

MicrofacetGGXVNDF::MicrofacetGGXVNDF(const Vec3& n, const Vec3& wo, Float a, Float t)
    : frame{ n }
    , wo{ wo }
    , alpha{ a }
    , t{ t }
{
}

#define SPHERICAL_CAPS_VNDF_SAMPLING 1

Vec3 MicrofacetGGXVNDF::Sample(const Point2& u0) const
{
    Point2 u = u0;
    if (u[0] < t)
    {
        u[0] /= t;

        Vec3 w = frame.ToLocal(wo);

#if SPHERICAL_CAPS_VNDF_SAMPLING
        Vec3 h = Sample_GGX_VNDF_Dupuy_Benyoub(w, alpha, u);
#else
        Vec3 h = Sample_GGX_VNDF_Heitz(wo, alpha, u);
#endif
        Vec3 wh = frame.FromLocal(h);
        Vec3 wi = Reflect(wo, wh);

        return wi;
    }
    else
    {
        u[0] = (u[0] - t) / (1 - t);

        Vec3 random_cosine = CosineSampleHemisphere(u);
        return frame.FromLocal(random_cosine);
    }
}

Float MicrofacetGGXVNDF::Evaluate(const Vec3& wi) const
{
    Float alpha2 = alpha * alpha;

    Vec3 h = Normalize(wo + wi);
    Float NoH = Dot(h, frame.z);
    Float LoH = Dot(/*L*/ wi, /*H*/ frame.z);
    Float spec_w = D_GGX(NoH, alpha2) * G1_Smith(LoH, alpha2) / std::fmax(4 * LoH, Float(0.0));

    Float diff_w = LoH * inv_pi;

    return (1 - t) * diff_w + t * spec_w;
}

} // namespace bulbit
