#pragma once

#include "aabb.h"

namespace bulbit
{

class Material;
struct Intersection;

class Intersectable
{
public:
    virtual ~Intersectable() = default;

    virtual void GetAABB(AABB* out_aabb) const = 0;
    virtual bool Intersect(Intersection* out_is, const Ray& ray, Float t_min, Float t_max) const = 0;
    virtual bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const = 0;

protected:
    friend class DynamicBVH;
};

struct Intersection
{
    Float t;

    const Material* material;

    Point3 point;
    Vec3 normal; // Geometric normal
    Point2 uv;

    bool front_face;

    struct
    {
        Vec3 normal, tangent;
    } shading;
};

} // namespace bulbit
