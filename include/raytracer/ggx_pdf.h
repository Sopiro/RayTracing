#pragma once

#include "pdf.h"

namespace spt
{

extern double D_GGX(double NoH, double alpha2);

class GGXPDF : public PDF
{
    // https://agraphicsguynotes.com/posts/sample_microfacet_brdf/
    // https://schuttejoe.github.io/post/ggximportancesamplingpart1/

public:
    GGXPDF(const Vec3& n, const Vec3& wi, double roughness, double t)
        : uvw{ n.Normalized() }
        , wi{ -wi.Normalized() }
        , alpha2{ roughness * roughness }
        , t{ t }
    {
    }

    virtual Vec3 Generate() const override
    {
        if (Rand() < t)
        {
            double u = Rand();
            double theta = acos(sqrt((1.0 - u) / ((alpha2 - 1.0) * u + 1.0)));
            double phi = 2.0 * pi * Rand();

            double sin_thetha = sin(theta);
            double x = cos(phi) * sin_thetha;
            double y = sin(phi) * sin_thetha;
            double z = cos(theta);

            // sampled half vector
            Vec3 h{ x, y, Abs(z) };
            Vec3 wo = Reflect(-wi, uvw.GetLocal(h));

            return wo;
        }
        else
        {
            Vec3 random_cosine = RandomCosineDirection();
            return uvw.GetLocal(random_cosine);
        }
    }

    virtual double Evaluate(const Vec3& wo) const override
    {
        Vec3 h = (wi + wo).Normalized();
        double NoH = Dot(uvw.w, h);
        double spec_w = D_GGX(NoH, alpha2) * NoH / (4.0 * Max(Dot(wo, h), epsilon));

        double cosine = Dot(wo, uvw.w);
        double diff_w = cosine / pi;

        return (1.0 - t) * diff_w + t * spec_w;
    }

public:
    ONB uvw;
    Vec3 wi;
    double alpha2;
    double t;
};

} // namespace spt
