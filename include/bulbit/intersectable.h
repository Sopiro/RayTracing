#pragma once

#include "bounding_box.h"
#include "spectrum.h"

namespace bulbit
{

class Material;
class Primitive;
class BSDF;
class BSSRDF;
class Medium;

struct Intersection
{
    const Primitive* primitive;

    Float t;
    Point3 point;
    Vec3 normal; // Geometric normal
    Point2 uv;

    bool front_face;

    struct
    {
        Vec3 normal, tangent;
    } shading;

    Spectrum Le(const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Vec3& wo, Allocator& alloc);
    bool GetBSSRDF(BSSRDF** bssrdf, const Vec3& wo, Allocator& alloc);
    const Medium* GetMedium(const Vec3& w) const;
};

class Intersectable
{
public:
    virtual ~Intersectable() = default;

    virtual AABB GetAABB() const = 0;
    virtual bool Intersect(Intersection* out_isect, const Ray& ray, Float t_min, Float t_max) const = 0;
    virtual bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const = 0;
};

} // namespace bulbit
