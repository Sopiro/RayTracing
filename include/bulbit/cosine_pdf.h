#pragma once

#include "pdf.h"
#include "sampling.h"

namespace spt
{

class CosinePDF : public PDF
{
public:
    CosinePDF(const Vec3& n);

    virtual Vec3 Sample() const override;
    virtual Float Evaluate(const Vec3& wi) const override;

public:
    ONB uvw;
};

inline CosinePDF::CosinePDF(const Vec3& n)
    : uvw{ n }
{
}

inline Vec3 CosinePDF::Sample() const
{
    return uvw.GetLocal(CosineSampleHemisphere());
}

inline Float CosinePDF::Evaluate(const Vec3& wi) const
{
    Float cosine = Dot(wi, uvw.w);
    return cosine <= 0 ? 0 : cosine * inv_pi;
}

} // namespace spt
