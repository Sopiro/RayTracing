#pragma once

#include "microfacet.h"
#include "pdf.h"

namespace spt
{

// GGX visible normal distribution function PDF
class GGXVNDFPDF : public PDF
{
    // https://hal.inria.fr/hal-01024289/file/Heitz2014Slides.pdf
    // https://jcgt.org/published/0003/02/03/
    // https://hal.inria.fr/hal-00996995v1/document
    // https://hal.archives-ouvertes.fr/hal-01509746/document
    // https://schuttejoe.github.io/post/ggximportancesamplingpart2/
    // https://simonstechblog.blogspot.com/2020/01/note-on-sampling-ggx-distribution-of.html

public:
    GGXVNDFPDF(const Vec3& n, const Vec3& wo, float64 roughness, float64 t);

    virtual Vec3 Generate() const override;
    virtual float64 Evaluate(const Vec3& d) const override;

public:
    ONB uvw;
    Vec3 wo;
    float64 alpha;
    float64 t;
};

inline GGXVNDFPDF::GGXVNDFPDF(const Vec3& n, const Vec3& wo, float64 roughness, float64 t)
    : uvw{ n }
    , wo{ wo }
    , alpha{ roughness }
    , t{ t }
{
}

inline Vec3 GGXVNDFPDF::Generate() const
{
    if (Rand() < t)
    {
        // Section 3.2: transforming the view direction to the hemisphere configuration
        Vec3 Vh{ alpha * wo.x, alpha * wo.y, wo.z };
        Vh.Normalize();

        // Build an orthonormal basis with v, t1, and t2
        // Section 4.1: orthonormal basis (with special case if cross product is zero)
        Vec3 T1 = (Vh.z < 0.999) ? (Cross(Vh, z_axis)).Normalized() : x_axis;
        Vec3 T2 = Cross(T1, Vh);

        float64 u1 = Rand();
        float64 u2 = Rand();

        // Section 4.2: parameterization of the projected area
        float64 r = sqrt(u1);
        float64 phi = two_pi * u2;
        float64 t1 = r * cos(phi);
        float64 t2 = r * sin(phi);
        float64 s = 0.5 * (1.0 + Vh.z);
        t2 = Lerp(sqrt(1.0 - t1 * t1), t2, s);

        // Section 4.3: reprojection onto hemisphere
        Vec3 Nh = t1 * T1 + t2 * T2 + sqrt(fmax(0.0, 1.0 - t1 * t1 - t2 * t2)) * Vh;

        // Section 3.4: transforming the normal back to the ellipsoid configuration
        Vec3 h = Vec3(alpha * Nh.x, alpha * Nh.y, fmax(0.0, Nh.z)).Normalized(); // Sampled half vector
        Vec3 wh = uvw.GetLocal(h);
        Vec3 wi = Reflect(-wo, wh);

        return wi;
    }
    else
    {
        Vec3 random_cosine = RandomCosineDirection();
        return uvw.GetLocal(random_cosine);
    }
}

inline float64 GGXVNDFPDF::Evaluate(const Vec3& d) const
{
    float64 alpha2 = alpha * alpha;

    Vec3 h = (wo + d).Normalized();
    float64 NoH = Dot(uvw.w, h);
    float64 NoV = Dot(uvw.w, d);
    float64 spec_w = D_GGX(NoH, alpha2) * G1_Smith(NoV, alpha2) / Max(4.0 * NoV, epsilon);

    float64 cosine = Dot(d, uvw.w);
    float64 diff_w = cosine * inv_pi;

    return (1.0 - t) * diff_w + t * spec_w;
}

} // namespace spt
