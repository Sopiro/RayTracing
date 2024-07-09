#pragma once

#include "intersectable.h"
#include "medium.h"

namespace bulbit
{

struct PrimitiveSample
{
    Point3 point;
    Vec3 normal;
    Float pdf;
};

// Represents a geometric primitive
class Primitive : public Intersectable
{
public:
    virtual ~Primitive() = default;

    // Returns random point on the surface
    virtual PrimitiveSample Sample(const Point2& u) const = 0;

    // Returns random point relative to the reference point
    virtual PrimitiveSample Sample(const Point3& ref, const Point2& u) const = 0;

    virtual Float EvaluatePDF(const Ray& ray) const = 0;
    virtual Float PDF(const Intersection& hit_is, const Ray& hit_ray) const = 0;

    virtual const Material* GetMaterial() const = 0;

protected:
    static inline void SetFaceNormal(
        Intersection* isect, const Vec3& wi, const Vec3& outward_normal, const Vec3& shading_normal, const Vec3& shading_tangent)
    {
        if (Dot(wi, outward_normal) < 0)
        {
            isect->front_face = true;
            isect->normal = outward_normal;
            isect->shading.normal = shading_normal;
            isect->shading.tangent = shading_tangent;
        }
        else
        {
            isect->front_face = false;
            isect->normal = -outward_normal;
            isect->shading.normal = -shading_normal;
            isect->shading.tangent = -shading_tangent;
        }
    }

    MediumInterface medium_interface;
};

} // namespace bulbit
