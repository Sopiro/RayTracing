#pragma once

#include "intersectable.h"
#include "onb.h"

namespace bulbit
{

// Directinal PDF
class BRDF
{
public:
    virtual ~BRDF() = default;

    // Given an outgoing direction wo, importance sample an incident direction
    virtual Vec3 Sample(const Point2& u) const = 0;

    // Evaluate PDF with given direction
    virtual Float Evaluate(const Vec3& wi) const = 0;
};

} // namespace bulbit