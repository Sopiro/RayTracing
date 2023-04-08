#pragma once

#include "microfacet.h"
#include "pdf.h"

namespace spt
{

// GGX normal distribution function PDF
class GGXPDF : public PDF
{
    // https://agraphicsguynotes.com/posts/sample_microfacet_brdf/
    // https://schuttejoe.github.io/post/ggximportancesamplingpart1/

public:
    GGXPDF(const Vec3& n, const Vec3& wo, double roughness, double t)
        : uvw{ n }
        , wo{ wo }
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
        Vec3 h = (wo + d).Normalized();
        double NoH = Dot(uvw.w, h);
        double spec_w = D_GGX(NoH, alpha2) * NoH / Max(4.0 * Dot(d, h), epsilon);

        double cosine = Dot(d, uvw.w);
        double diff_w = cosine / pi;

        return (1.0 - t) * diff_w + t * spec_w;
    }

public:
    ONB uvw;
    Vec3 wo;
    double alpha2;
    double t;
};

} // namespace spt
