#pragma once

#include "brdf.h"

namespace bulbit
{

// GGX + Lambertian
class MicrofacetGGX : public BRDF
{
public:
    MicrofacetGGX(const Vec3& n, const Vec3& wo, Float a, Float t);

    virtual Vec3 Sample(const Point2& u) const override;
    virtual Float Evaluate(const Vec3& wi) const override;

private:
    ONB uvw;
    Vec3 wo;
    Float alpha2;
    Float t;
};

inline MicrofacetGGX::MicrofacetGGX(const Vec3& n, const Vec3& wo, Float a, Float t)
    : uvw{ n }
    , wo{ wo }
    , alpha2{ a * a }
    , t{ t }
{
}

} // namespace bulbit
