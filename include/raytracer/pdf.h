#pragma once

#include "common.h"
#include "onb.h"

namespace spt
{

// Probability distribution function
class PDF
{
public:
    virtual ~PDF() = default;

    virtual double Value(const Vec3& direction) const = 0;
    virtual Vec3 Generate() const = 0;
};

class CosinePDF : public PDF
{
public:
    CosinePDF(const Vec3& w)
    {
        uvw.BuildFromW(w);
    }

    virtual double Value(const Vec3& direction) const override
    {
        double cosine = Dot(direction.Normalized(), uvw.w);
        return (cosine <= 0.0) ? 0.0 : cosine / pi;
    }

    virtual Vec3 Generate() const override
    {
        return uvw.GetLocal(RandomCosineDirection());
    }

public:
    ONB uvw;
};

class HittablePDF : public PDF
{
public:
    HittablePDF(std::shared_ptr<Hittable> target, const Vec3& origin)
        : target{ target }
        , origin{ origin }
    {
    }

    // Returns random direction vector hitting this object
    virtual Vec3 Generate() const override
    {
        return target->Random(origin);
    }

    virtual double Value(const Vec3& direction) const override
    {
        HitRecord rec;
        if (target->Hit(Ray{ origin, direction }, 0.00001, infinity, rec))
        {
            return rec.object->PDFValue(direction, rec);
        }

        return 0.0;
    }

public:
    Vec3 origin;
    std::shared_ptr<Hittable> target;
};

} // namespace spt