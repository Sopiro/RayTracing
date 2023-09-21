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
    virtual void Sample(Intersection* sample, f64* pdf) const = 0;

    // Returns random point relative to the reference point
    virtual void Sample(Intersection* sample, f64* pdf, Vec3* ref2p, const Point3& ref) const = 0;

    virtual f64 EvaluatePDF(const Ray& ray) const = 0;

    virtual const Material* GetMaterial() const = 0;
};

inline void SetFaceNormal(
    Intersection* is, const Vec3& wi, const Vec3& outward_normal, const Vec3& shading_normal, const Vec3& shading_tangent)
{
    is->shading.normal = shading_normal;
    is->shading.tangent = shading_tangent;

    if (Dot(wi, outward_normal) < 0.0)
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

} // namespace spt
