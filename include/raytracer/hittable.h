#pragma once

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

    inline void SetFaceNormal(const Ray& r, const Vec3& outward_normal)
    {
        front_face = Dot(r.dir, outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable
{
public:
    virtual bool Hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
};
