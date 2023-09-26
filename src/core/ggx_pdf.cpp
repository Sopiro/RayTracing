#include "spt/ggx_pdf.h"

namespace spt
{

Vec3 GGXPDF::Sample() const
{
    if (Rand() < t)
    {
        Vec2 u = RandVec2();
        Vec3 h = Sample_GGX(wo, alpha2, u);

        Vec3 wh = uvw.GetLocal(h);
        Vec3 wi = Reflect(wo, wh);

        return wi;
    }
    else
    {
        Vec3 random_cosine = CosineSampleHemisphere();
        return uvw.GetLocal(random_cosine);
    }
}

f64 GGXPDF::Evaluate(const Vec3& wi) const
{
    Vec3 h = Normalize(wo + wi);
    f64 NoH = Dot(h, uvw.w);
    f64 LoH = Dot(/*L*/ wi, /*H*/ uvw.w);
    f64 spec_w = D_GGX(NoH, alpha2) * NoH / std::fmax(4.0 * LoH, 0.0);

    f64 diff_w = LoH * inv_pi;

    return (1.0 - t) * diff_w + t * spec_w;
}

} // namespace spt
