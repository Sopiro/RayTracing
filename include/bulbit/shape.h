#pragma once

#include "aabb.h"
#include "intersectable.h"
#include "math.h"
#include "ray.h"

namespace bulbit
{

inline void SetFaceNormal(
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

struct ShapeSample
{
    Point3 point;
    Vec3 normal;
    Float pdf;
};

class Shape : public Intersectable
{
public:
    virtual AABB GetAABB() const = 0;

    // Returns random point on the surface
    virtual ShapeSample Sample(const Point2& u) const = 0;

    // Returns random point relative to the reference point
    virtual ShapeSample Sample(const Point3& ref, const Point2& u) const = 0;

    virtual Float EvaluatePDF(const Ray& ray) const = 0;
    virtual Float PDF(const Intersection& hit_is, const Ray& hit_ray) const = 0;
};

} // namespace bulbit
