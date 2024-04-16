#pragma once

#include "aabb.h"

namespace bulbit
{

class Material;

struct Intersection
{
    Float t;

    Point3 point;
    Vec3 normal; // Geometric normal
    Point2 uv;
    const Material* material;

    bool front_face;

    struct
    {
        Vec3 normal, tangent;
    } shading;
};

class Intersectable
{
public:
    virtual ~Intersectable() = default;

    virtual AABB GetAABB() const = 0;
    virtual bool Intersect(Intersection* out_is, const Ray& ray, Float t_min, Float t_max) const = 0;
    virtual bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const = 0;
};

} // namespace bulbit
