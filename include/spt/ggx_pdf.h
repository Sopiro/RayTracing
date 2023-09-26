#pragma once

#include "brdf.h"
#include "pdf.h"

namespace spt
{

// GGX normal distribution function PDF
class GGXPDF : public PDF
{
    // https://agraphicsguynotes.com/posts/sample_microfacet_brdf/
    // https://schuttejoe.github.io/post/ggximportancesamplingpart1/

public:
    GGXPDF(const Vec3& n, const Vec3& wo, f64 a, f64 t);

    virtual Vec3 Sample() const override;
    virtual f64 Evaluate(const Vec3& wi) const override;

private:
    ONB uvw;
    Vec3 wo;
    f64 alpha2;
    f64 t;
};

inline GGXPDF::GGXPDF(const Vec3& n, const Vec3& wo, f64 a, f64 t)
    : uvw{ n }
    , wo{ wo }
    , alpha2{ a * a }
    , t{ t }
{
}

} // namespace spt
