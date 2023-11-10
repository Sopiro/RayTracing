#pragma once

#include "brdf.h"
#include "sampling.h"

namespace bulbit
{

class LambertianReflection : public BRDF
{
public:
    LambertianReflection(const Vec3& n);

    virtual Vec3 Sample(const Point2& u) const override;
    virtual Float Evaluate(const Vec3& wi) const override;

public:
    ONB uvw;
};

inline LambertianReflection::LambertianReflection(const Vec3& n)
    : uvw{ n }
{
}

inline Vec3 LambertianReflection::Sample(const Point2& u) const
{
    return uvw.FromLocal(CosineSampleHemisphere(u));
}

inline Float LambertianReflection::Evaluate(const Vec3& wi) const
{
    Float cosine = Dot(wi, uvw.w);
    return cosine <= 0 ? 0 : cosine * inv_pi;
}

} // namespace bulbit
