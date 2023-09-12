#pragma once

#include "intersectable.h"

namespace spt
{

// Represents a geometric primitive
class Primitive : public Intersectable
{
public:
    virtual ~Primitive() = default;

    // Returns random point on the surface
    virtual Point3 Sample() const = 0;

    // Returns random point relative to the reference point
    virtual Point3 Sample(const Point3& ref) const = 0;

    virtual f64 EvaluatePDF(const Ray& ray) const = 0;

    // Input ray must hit this object
    virtual f64 PDFValue(const Intersection& hit_is, const Ray& hit_ray) const = 0;

    virtual const Material* GetMaterial() const = 0;
};

inline void SetFaceNormal(Intersection* is, const Ray& ray, const Vec3& outward_normal, const Vec3& outward_tangent)
{
    if (Dot(ray.dir, outward_normal) < 0.0)
    {
        is->front_face = true;
        is->normal = outward_normal;
        is->tangent = outward_tangent;
    }
    else
    {
        is->front_face = false;
        is->normal = -outward_normal;
        is->tangent = -outward_tangent;
    }
}

} // namespace spt
