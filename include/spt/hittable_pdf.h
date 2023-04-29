#pragma once

#include "hittable.h"
#include "pdf.h"

namespace spt
{

class HittablePDF : public PDF
{
public:
    HittablePDF(const Hittable* target, const Vec3& origin);

    // Returns random direction vector hitting this object
    virtual Vec3 Generate() const override;

    // Evaluate pdf value with given direction from origin
    virtual double Evaluate(const Vec3& direction) const override;

public:
    Vec3 origin;
    const Hittable* target;
};

inline HittablePDF::HittablePDF(const Hittable* target, const Vec3& origin)
    : target{ target }
    , origin{ origin }
{
}

// Returns random direction vector hitting this object
inline Vec3 HittablePDF::Generate() const
{
    return target->GetRandomDirection(origin);
}

inline double HittablePDF::Evaluate(const Vec3& direction) const
{
    return target->EvaluatePDF(Ray{ origin, direction });
}

} // namespace spt
