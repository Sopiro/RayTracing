#pragma once

#include "common.h"
#include "intersectable.h"
#include "onb.h"

namespace spt
{

// Heuristic functions for MIS
inline f64 BalanceHeuristic(f64 pdf_f, f64 pdf_g)
{
    return pdf_f / (pdf_f + pdf_g);
}

inline f64 PowerHeuristic(f64 pdf_f, f64 pdf_g)
{
    f64 f2 = pdf_f * pdf_f;
    f64 g2 = pdf_g * pdf_g;
    return f2 / (f2 + g2);
}

// Directinal PDF(Probability Distribution Function)
class PDF
{
public:
    virtual ~PDF() = default;

    // Given an outgoing direction wo, importance sample an incident direction
    virtual Vec3 Sample() const = 0;

    // Evaluate PDF with given direction
    virtual f64 Evaluate(const Vec3& wi) const = 0;
};

} // namespace spt