#pragma once

#include "brdf.h"
#include "pdf.h"

namespace bulbit
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
    // https://cdrdv2-public.intel.com/782052/sampling-visible-ggx-normals.pdf

public:
    GGXVNDFPDF(const Vec3& n, const Vec3& wo, Float a, Float t);

    virtual Vec3 Sample(const Point2& u) const override;
    virtual Float Evaluate(const Vec3& wi) const override;

private:
    ONB uvw;
    Vec3 wo;
    Float alpha;
    Float t;
};

inline GGXVNDFPDF::GGXVNDFPDF(const Vec3& n, const Vec3& wo, Float a, Float t)
    : uvw{ n }
    , wo{ wo }
    , alpha{ a }
    , t{ t }
{
}

} // namespace bulbit
