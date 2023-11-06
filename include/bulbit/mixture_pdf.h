#pragma once

#include "brdf.h"

namespace bulbit
{

class MixturePDF : public BRDF
{
public:
    MixturePDF(BRDF* pdf1, BRDF* pdf2);

    virtual Vec3 Sample(const Point2& u) const override;
    virtual Float Evaluate(const Vec3& wi) const override;

public:
    BRDF* p1;
    BRDF* p2;
};

inline MixturePDF::MixturePDF(BRDF* pdf1, BRDF* pdf2)
    : p1{ pdf1 }
    , p2{ pdf2 }
{
}

inline Vec3 MixturePDF::Sample(const Point2& u0) const
{
    Point2 u = u0;
    if (u[0] > Float(0.5))
    {
        u[0] = 2 * u[0];
        return p1->Sample(u);
    }
    else
    {
        u[0] = 2 * (u[0] - Float(0.5));
        return p2->Sample(u);
    }
}

inline Float MixturePDF::Evaluate(const Vec3& wi) const
{
    // Mixing two pdfs
    return Float(0.5) * (p1->Evaluate(wi) + p2->Evaluate(wi));
}

} // namespace bulbit
