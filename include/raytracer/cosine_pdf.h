#pragma once

#include "pdf.h"

namespace spt
{

class CosinePDF : public PDF
{
public:
    CosinePDF(const Vec3& w)
        : uvw{ w }
    {
    }

    virtual Vec3 Generate() const override
    {
        return uvw.GetLocal(RandomCosineDirection());
    }

    virtual double Evaluate(const Vec3& direction) const override
    {
        double cosine = Dot(direction.Normalized(), uvw.w);
        return (cosine <= 0.0) ? 0.0 : cosine / pi;
    }

public:
    ONB uvw;
};

} // namespace spt
