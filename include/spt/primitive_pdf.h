#pragma once

#include "pdf.h"
#include "primitive.h"

namespace spt
{

class PrimitivePDF : public PDF
{
public:
    PrimitivePDF(const Primitive* target, const Vec3& origin);

    // Returns random direction vector hitting this object
    virtual Vec3 Sample() const override;

    // Evaluate pdf value with given incident direction from origin
    virtual f64 Evaluate(const Vec3& wi) const override;

public:
    Vec3 origin;
    const Primitive* target;
};

inline PrimitivePDF::PrimitivePDF(const Primitive* target, const Vec3& origin)
    : target{ target }
    , origin{ origin }
{
}

// Returns random direction vector hitting this object
inline Vec3 PrimitivePDF::Sample() const
{
    Point3 point = target->Sample(origin);
    return point - origin;
}

inline f64 PrimitivePDF::Evaluate(const Vec3& wi) const
{
    return target->EvaluatePDF(Ray{ origin, wi });
}

} // namespace spt
