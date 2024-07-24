#pragma once

#include "bounding_box.h"
#include "intersectable.h"
#include "math.h"
#include "ray.h"

namespace bulbit
{

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

protected:
    static void SetFaceNormal(
        Intersection* isect, const Vec3& wi, const Vec3& outward_normal, const Vec3& shading_normal, const Vec3& shading_tangent
    )
    {
        bool front_face = Dot(wi, outward_normal) < 0;
        Float sign = front_face ? 1 : -1;

        isect->front_face = front_face;
        isect->normal = sign * outward_normal;
        isect->shading.normal = sign * shading_normal;
        isect->shading.tangent = sign * shading_tangent;
    }
};

} // namespace bulbit
