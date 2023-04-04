#pragma once

#include "hittable.h"
#include "pdf.h"

namespace spt
{

class HittablePDF : public PDF
{
public:
    HittablePDF(const Hittable* target, const Vec3& origin)
        : target{ target }
        , origin{ origin }
    {
    }

    // Returns random direction vector hitting this object
    virtual Vec3 Generate() const override
    {
        return target->GetRandomDirection(origin);
    }

    virtual double Evaluate(const Vec3& direction) const override
    {
        return target->EvaluatePDF(Ray{ origin, direction });
    }

public:
    Vec3 origin;
    const Hittable* target;
};

} // namespace spt
