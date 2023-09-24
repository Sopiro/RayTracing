#pragma once

#include "aabb.h"

namespace spt
{

using NodeProxy = i32;

class Material;
struct Intersection;

class Intersectable
{
public:
    virtual ~Intersectable() = default;

    virtual bool Intersect(Intersection* out_is, const Ray& ray, f64 t_min, f64 t_max) const = 0;
    virtual bool IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const = 0;
    virtual void GetAABB(AABB* out_aabb) const = 0;

protected:
    friend class BVH;

    // BVH node proxy
    NodeProxy node;
};

struct Intersection
{
    f64 t;

    const Intersectable* object;
    const Material* material;

    Point3 point;
    Vec3 normal; // Geometric normal
    UV uv;

    bool front_face;

    struct
    {
        Vec3 normal, tangent;
    } shading;
};

} // namespace spt
