#include "spt/ggxvndf_pdf.h"

#define SPHERICAL_CAPS_VNDF_SAMPLING 1

namespace spt
{

Vec3 GGXVNDFPDF::Sample() const
{
    if (Rand() < t)
    {
        Vec2 u = RandVec2();

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
        Vec3 random_cosine = CosineSampleHemisphere();
        return uvw.GetLocal(random_cosine);
    }
}

f64 GGXVNDFPDF::Evaluate(const Vec3& wi) const
{
    f64 alpha2 = alpha * alpha;

    Vec3 h = Normalize(wo + wi);
    f64 NoH = Dot(h, uvw.w);
    f64 LoH = Dot(/*L*/ wi, /*H*/ uvw.w);
    f64 spec_w = D_GGX(NoH, alpha2) * G1_Smith(LoH, alpha2) / std::fmax(4.0 * LoH, 0.0);

    f64 diff_w = LoH * inv_pi;

    return (1.0 - t) * diff_w + t * spec_w;
}

} // namespace spt
