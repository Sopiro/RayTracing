#pragma once

#include "intersectable.h"
#include "pdf.h"

namespace spt
{

class IntersectablePDF : public PDF
{
public:
    IntersectablePDF(const Intersectable* target, const Vec3& origin);

    // Returns random direction vector hitting this object
    virtual Vec3 Sample() const override;

    // Evaluate pdf value with given incident direction from origin
    virtual f64 Evaluate(const Vec3& wi) const override;

public:
    Vec3 origin;
    const Intersectable* target;
};

inline IntersectablePDF::IntersectablePDF(const Intersectable* target, const Vec3& origin)
    : target{ target }
    , origin{ origin }
{
}

// Returns random direction vector hitting this object
inline Vec3 IntersectablePDF::Sample() const
{
    return target->GetRandomDirection(origin);
}

inline f64 IntersectablePDF::Evaluate(const Vec3& wi) const
{
    return target->EvaluatePDF(Ray{ origin, wi });
}

} // namespace spt
