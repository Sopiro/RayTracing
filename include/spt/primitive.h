#pragma once

#include "intersectable.h"

namespace spt
{

struct SurfaceSample
{
    Point3 p;
    Vec3 n;
    f64 pdf;
};

// Represents a geometric primitive
class Primitive : public Intersectable
{
public:
    virtual ~Primitive() = default;

    // Returns random point on the surface
    virtual void Sample(SurfaceSample* sample) const = 0;

    // Returns random point relative to the reference point
    virtual void Sample(SurfaceSample* sample, Vec3* ref2p, const Point3& ref) const = 0;

    virtual f64 EvaluatePDF(const Ray& ray) const = 0;

    virtual const Material* GetMaterial() const = 0;
};

inline void SetFaceNormal(Intersection* is, const Ray& ray, const Vec3& outward_normal, const Vec3& outward_tangent)
{
    if (Dot(ray.d, outward_normal) < 0.0)
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
