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

public:
    GGXVNDFPDF(const Vec3& n, const Vec3& wo, double roughness, double t)
        : uvw{ n }
        , wo{ wo }
        , alpha{ roughness }
        , t{ t }
    {
    }

    virtual Vec3 Generate() const override
    {
        if (Rand() < t)
        {
            // Stretch
            // Section 3.2: transforming the view direction to the hemisphere configuration
            Vec3 Vh{ alpha * wo.x, alpha * wo.y, wo.z };
            Vh.Normalize();

            // Build an orthonormal basis with v, t1, and t2
            // Section 4.1: orthonormal basis (with special case if cross product is zero)
            Vec3 T1 = (Vh.z < 0.999) ? (Cross(Vh, z_axis)).Normalized() : x_axis;
            Vec3 T2 = Cross(T1, Vh);

            double u1 = Rand();
            double u2 = Rand();

            // Section 4.2: parameterization of the projected area
            double r = sqrt(u1);
            double phi = 2.0 * pi * u2;
            double t1 = r * cos(phi);
            double t2 = r * sin(phi);
            double s = 0.5 * (1.0 + Vh.z);
            t2 = Lerp(sqrt(1.0 - t1 * t1), t2, s);

            // Section 4.3: reprojection onto hemisphere
            Vec3 Nh = t1 * T1 + t2 * T2 + sqrt(Max(0.0, 1.0 - t1 * t1 - t2 * t2)) * Vh;

            // Unstretch
            // Section 3.4: transforming the normal back to the ellipsoid configuration
            Vec3 h = Vec3(alpha * Nh.x, alpha * Nh.y, Max(0.0, Nh.z)).Normalized();
            return Reflect(-wo, uvw.GetLocal(h));
        }
        else
        {
            Vec3 random_cosine = RandomCosineDirection();
            return uvw.GetLocal(random_cosine);
        }
    }

    virtual double Evaluate(const Vec3& d) const override
    {
        double alpha2 = alpha * alpha;

        Vec3 h = (wo + d).Normalized();
        double NoH = Dot(uvw.w, h);
        double NoV = Dot(uvw.w, d);
        double spec_w = D_GGX(NoH, alpha2) * G1_Smith(NoV, alpha2) / Max(4.0 * NoV, epsilon);

        double cosine = Dot(d, uvw.w);
        double diff_w = cosine / pi;

        return (1.0 - t) * diff_w + t * spec_w;
    }

public:
    ONB uvw;
    Vec3 wo;
    double alpha;
    double t;
};

} // namespace spt
