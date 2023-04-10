#pragma once

#include "pdf.h"

namespace spt
{

class CosinePDF : public PDF
{
public:
    CosinePDF(const Vec3& n)
        : uvw{ n }
    {
    }

    virtual Vec3 Generate() const override
    {
        return uvw.GetLocal(RandomCosineDirection());
    }

    virtual double Evaluate(const Vec3& d) const override
    {
        double cosine = Dot(d, uvw.w);
        return cosine <= 0.0 ? 0.0 : cosine * inv_pi;
    }

public:
    ONB uvw;
};

} // namespace spt
