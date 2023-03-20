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
    Vec3 normal;
    double t;
    UV uv;
    bool front_face;

    std::shared_ptr<Material> mat;
    const Hittable* object;

    void SetFaceNormal(const Ray& ray, const Vec3& outward_normal)
    {
        front_face = Dot(ray.dir, outward_normal) < 0.0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable
{
public:
    virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const = 0;
    virtual bool GetAABB(AABB& outAABB) const = 0;

    virtual double EvaluatePDF(const Vec3& origin, const Vec3& dir) const
    {
        assert(false);
        return 0.0;
    }

    virtual double PDFValue(const Vec3& origin, const Vec3& dir, const HitRecord& rec) const
    {
        assert(false);
        return 0.0;
    }

    // Returns random direction toward this object
    virtual Vec3 GetRandomDirection(const Vec3& origin) const
    {
        assert(false);
        return Vec3{ 0, 0, 0 };
    }

protected:
    NodeProxy node;
};

} // namespace spt
