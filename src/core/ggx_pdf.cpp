#include "spt/ggx_pdf.h"

namespace spt
{

Vec3 GGXPDF::Sample() const
{
    if (Rand() < t)
    {
        Vec2 u = RandVec2();

        f64 theta = std::acos(std::sqrt((1.0 - u.x) / ((alpha2 - 1.0) * u.x + 1.0)));
        f64 phi = two_pi * u.y;

        f64 sin_thetha = std::sin(theta);
        f64 x = std::cos(phi) * sin_thetha;
        f64 y = std::sin(phi) * sin_thetha;
        f64 z = std::cos(theta);

        assert(z > 0.0);

        Vec3 h{ x, y, z }; // Sampled half vector
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
