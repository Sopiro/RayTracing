#pragma once

#include "aabb.h"

namespace spt
{

using NodeProxy = int32;

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

    // BVH node proxy
    NodeProxy node;
};

struct Intersection
{
    Float t;

    const Intersectable* object;
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

} // namespace spt
