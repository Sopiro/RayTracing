#pragma once

#include "aabb.h"
#include "common.h"
#include "ray.h"

namespace spt
{

typedef int32 NodeProxy;
class Material;
class Hittable;

struct HitRecord
{
    Vec3 point;
    Vec3 normal, tangent;
    UV uv;

    double t;
    bool front_face;

    const Hittable* object;
    const Material* mat;

    void SetFaceNormal(const Ray& ray, const Vec3& outward_normal, const Vec3& outward_tangent)
    {
        front_face = Dot(ray.dir, outward_normal) < 0.0;
        normal = front_face ? outward_normal : -outward_normal;
        tangent = front_face ? outward_tangent : -outward_tangent;
    }
};

class Hittable
{
public:
    virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const = 0;
    virtual bool GetAABB(AABB& outAABB) const = 0;

    virtual double EvaluatePDF(const Ray& ray) const
    {
        assert(false);
        return 0.0;
    }

    // Ray must hit this object
    virtual double PDFValue(const Ray& hit_ray, const HitRecord& hit_rec) const
    {
        assert(false);
        return 0.0;
    }

    // Returns random direction toward this object
    virtual Vec3 GetRandomDirection(const Vec3& origin) const
    {
        assert(false);
        return zero_vec3;
    }

    virtual void Rebuild()
    {
    }

protected:
    // BVH node proxy
    NodeProxy node;
};

} // namespace spt
