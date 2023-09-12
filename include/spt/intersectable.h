#pragma once

#include "aabb.h"
#include "common.h"
#include "ray.h"

namespace spt
{

using NodeProxy = i32;

class Material;
class Primitive;
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
    bool front_face;

    const Intersectable* object;
    const Material* material;

    Point3 point;
    Vec3 normal, tangent;
    UV uv;
};

} // namespace spt
