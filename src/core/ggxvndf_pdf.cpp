#include "bulbit/ggxvndf_pdf.h"
#include "bulbit/microfacet.h"
#include "bulbit/sampling.h"

#define SPHERICAL_CAPS_VNDF_SAMPLING 1

namespace bulbit
{

Vec3 MicrofacetGGXVNDF::Sample(const Point2& u0) const
{
    Point2 u = u0;
    if (u[0] < t)
    {
        u[0] /= t;

#if SPHERICAL_CAPS_VNDF_SAMPLING
        Vec3 h = Sample_GGX_VNDF_Dupuy_Benyoub(wo, alpha, u);
#else
        Vec3 h = Sample_GGX_VNDF_Heitz(wo, alpha, u);
#endif
        Vec3 wh = uvw.GetLocal(h);
        Vec3 wi = Reflect(wo, wh);

        return wi;
    }
    else
    {
        u[0] = (u[0] - t) / (1 - t);

        Vec3 random_cosine = CosineSampleHemisphere(u);
        return uvw.GetLocal(random_cosine);
    }
}

Float MicrofacetGGXVNDF::Evaluate(const Vec3& wi) const
{
    Float alpha2 = alpha * alpha;

    Vec3 h = Normalize(wo + wi);
    Float NoH = Dot(h, uvw.w);
    Float LoH = Dot(/*L*/ wi, /*H*/ uvw.w);
    Float spec_w = D_GGX(NoH, alpha2) * G1_Smith(LoH, alpha2) / std::fmax(4 * LoH, Float(0.0));

    Float diff_w = LoH * inv_pi;

    return (1 - t) * diff_w + t * spec_w;
}

} // namespace bulbit
