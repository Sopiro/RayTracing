#pragma once

#include "pdf.h"

namespace spt
{

class CosinePDF : public PDF
{
public:
    CosinePDF(const Vec3& n);

    virtual Vec3 Generate() const override;
    virtual float64 Evaluate(const Vec3& d) const override;

public:
    ONB uvw;
};

inline CosinePDF::CosinePDF(const Vec3& n)
    : uvw{ n }
{
}

inline Vec3 CosinePDF::Generate() const
{
    return uvw.GetLocal(RandomCosineDirection());
}

inline float64 CosinePDF::Evaluate(const Vec3& d) const
{
    float64 cosine = Dot(d, uvw.w);
    return cosine <= 0.0 ? 0.0 : cosine * inv_pi;
}

} // namespace spt
