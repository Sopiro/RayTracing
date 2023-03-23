#pragma once

#include "common.h"
#include "hittable.h"
#include "onb.h"

namespace spt
{

// Probability distribution function
class PDF
{
public:
    virtual ~PDF() = default;

    virtual Vec3 Generate() const = 0;
    virtual double Evaluate(const Vec3& direction) const = 0;
};

class CosinePDF : public PDF
{
public:
    CosinePDF(const Vec3& w)
        : uvw{ w }
    {
    }

    virtual Vec3 Generate() const override
    {
        return uvw.GetLocal(RandomCosineDirection());
    }

    virtual double Evaluate(const Vec3& direction) const override
    {
        double cosine = Dot(direction.Normalized(), uvw.w);
        return (cosine <= 0.0) ? 0.0 : cosine / pi;
    }

public:
    ONB uvw;
};

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

class MixturePDF : public PDF
{
public:
    MixturePDF(PDF* pdf1, PDF* pdf2)
        : p1{ pdf1 }
        , p2{ pdf2 }
    {
    }

    virtual Vec3 Generate() const override
    {
        if (Rand() > 0.5)
        {
            return p1->Generate();
        }
        else
        {
            return p2->Generate();
        }
    }

    virtual double Evaluate(const Vec3& direction) const override
    {
        // Mixing two pdfs
        return 0.5 * p1->Evaluate(direction) + 0.5 * p2->Evaluate(direction);
    }

public:
    PDF* p1;
    PDF* p2;
};

} // namespace spt