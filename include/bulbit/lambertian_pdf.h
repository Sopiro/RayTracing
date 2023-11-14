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

private:
    Frame frame;
};

inline LambertianReflection::LambertianReflection(const Vec3& n)
    : frame{ n }
{
}

inline Vec3 LambertianReflection::Sample(const Point2& u) const
{
    return frame.FromLocal(CosineSampleHemisphere(u));
}

inline Float LambertianReflection::Evaluate(const Vec3& wi) const
{
    Float cosine = Dot(wi, frame.z);
    return cosine <= 0 ? 0 : cosine * inv_pi;
}

} // namespace bulbit
