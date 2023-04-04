#pragma once

#include "common.h"
#include "hittable.h"
#include "onb.h"

namespace spt
{

inline double BalanceHeuristic(double pdf_f, double pdf_g)
{
    return pdf_f / (pdf_f + pdf_g);
}

inline double PowerHeuristic(double pdf_f, double pdf_g)
{
    double f2 = pdf_f * pdf_f;
    double g2 = pdf_g * pdf_g;
    return f2 / (f2 + g2);
}

// Probability distribution function
class PDF
{
public:
    virtual ~PDF() = default;

    virtual Vec3 Generate() const = 0;
    virtual double Evaluate(const Vec3& direction) const = 0;
};

} // namespace spt