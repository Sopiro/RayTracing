#pragma once

#include "common.h"
#include "hittable.h"
#include "onb.h"

namespace spt
{

// Heuristic functions for MIS
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

    // Given an outgoing direction wo, importance sample an incident direction
    virtual Vec3 Generate() const = 0;

    // Evaluate PDF with given direction
    virtual double Evaluate(const Vec3& d) const = 0;
};

} // namespace spt