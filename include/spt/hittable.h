#pragma once

#include "aabb.h"
#include "common.h"
#include "ray.h"

namespace spt
{

typedef i32 NodeProxy;
class Material;
class Hittable;

struct HitRecord
{
    void SetFaceNormal(const Ray& ray, const Vec3& outward_normal, const Vec3& outward_tangent)
    {
        front_face = Dot(ray.dir, outward_normal) < 0.0;
        normal = front_face ? outward_normal : -outward_normal;
        tangent = front_face ? outward_tangent : -outward_tangent;
    }

    Point3 point;
    Vec3 normal, tangent;
    UV uv;

    f64 t;
    bool front_face;

    const Hittable* object;
    const Material* mat;
};

class Hittable
{
public:
    virtual bool Hit(const Ray& ray, f64 t_min, f64 t_max, HitRecord& rec) const = 0;
    virtual bool GetAABB(AABB& outAABB) const = 0;

    virtual f64 EvaluatePDF(const Ray& ray) const;

    // Input ray must hit this object
    virtual f64 PDFValue(const Ray& hit_ray, const HitRecord& hit_rec) const;

    // Returns random direction toward this object
    virtual Vec3 GetRandomDirection(const Point3& origin) const;

    // Returns the total object count
    virtual i32 GetSize() const;

    virtual void Rebuild();

protected:
    // BVH node proxy
    NodeProxy node;
};

inline f64 Hittable::EvaluatePDF(const Ray& ray) const
{
    assert(false);
    return 0.0;
}

// Ray must hit this object
inline f64 Hittable::PDFValue(const Ray& hit_ray, const HitRecord& hit_rec) const
{
    assert(false);
    return 0.0;
}

// Returns random direction toward this object
inline Vec3 Hittable::GetRandomDirection(const Point3& origin) const
{
    assert(false);
    return zero_vec3;
}

inline i32 Hittable::GetSize() const
{
    assert(false);
    return 0;
}

inline void Hittable::Rebuild()
{
    assert(false);
}

} // namespace spt
