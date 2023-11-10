#include "bulbit/microfacet.h"
#include "bulbit/sampling.h"

namespace bulbit
{

Vec3 MicrofacetGGX::Sample(const Point2& u0) const
{
    Point2 u = u0;
    if (u[0] < t)
    {
        u[0] /= t;

        Vec3 w = uvw.ToLocal(wo);
        Vec3 h = Sample_GGX(w, alpha2, u);
        Vec3 wh = uvw.FromLocal(h);
        Vec3 wi = Reflect(wo, wh);

        return wi;
    }
    else
    {
        u[0] = (u[0] - t) / (1 - t);

        Vec3 random_cosine = CosineSampleHemisphere(u);
        return uvw.FromLocal(random_cosine);
    }
}

Float MicrofacetGGX::Evaluate(const Vec3& wi) const
{
    Vec3 h = Normalize(wo + wi);
    Float NoH = Dot(h, uvw.w);
    Float LoH = Dot(/*L*/ wi, /*H*/ uvw.w);
    Float spec_w = D_GGX(NoH, alpha2) * NoH / std::fmax(4 * LoH, Float(0.0));

    Float diff_w = LoH * inv_pi;

    return (1 - t) * diff_w + t * spec_w;
}

} // namespace bulbit
