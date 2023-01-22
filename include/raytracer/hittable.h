#pragma once

#include "aabb.h"
#include "common.h"
#include "ray.h"

class Material;

struct HitRecord
{
    Vec3 p;
    Vec3 normal;
    double t;
    UV uv;
    bool front_face;

    std::shared_ptr<Material> mat;

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

    int32 node;
};
