#pragma once

#include "intersectable.h"

namespace bulbit
{

// Represents a geometric primitive
class Primitive : public Intersectable
{
public:
    virtual ~Primitive() = default;

    virtual Primitive* Clone(Allocator* allocator) const = 0;

    // Returns random point on the surface
    virtual void Sample(Intersection* sample, Float* pdf, const Point2& u) const = 0;

    // Returns random point relative to the reference point
    virtual void Sample(Intersection* sample, Float* pdf, Vec3* ref2p, const Point3& ref, const Point2& u) const = 0;

    virtual Float EvaluatePDF(const Ray& ray) const = 0;
    virtual Float PDFValue(const Intersection& hit_is, const Ray& hit_ray) const = 0;

    virtual MaterialIndex GetMaterialIndex() const = 0;

protected:
    static void SetFaceNormal(Intersection* is, const Vec3& wi, const Vec3& outward_normal);
};

inline void Primitive::SetFaceNormal(Intersection* is, const Vec3& wi, const Vec3& outward_normal)
{
    if (Dot(wi, outward_normal) < 0)
    {
        is->front_face = true;
        is->normal = outward_normal;
    }
    else
    {
        is->front_face = false;
        is->normal = -outward_normal;
    }
}

} // namespace bulbit
