#pragma once

#include "brdf.h"
#include "pdf.h"

namespace bulbit
{

// GGX VNDF + Lambertian
class MicrofacetGGXVNDF : public BRDF
{
public:
    MicrofacetGGXVNDF(const Vec3& n, const Vec3& wo, Float a, Float t);

    virtual Vec3 Sample(const Point2& u) const override;
    virtual Float Evaluate(const Vec3& wi) const override;

private:
    ONB uvw;
    Vec3 wo;
    Float alpha;
    Float t;
};

inline MicrofacetGGXVNDF::MicrofacetGGXVNDF(const Vec3& n, const Vec3& wo, Float a, Float t)
    : uvw{ n }
    , wo{ wo }
    , alpha{ a }
    , t{ t }
{
}

} // namespace bulbit
