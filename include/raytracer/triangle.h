#pragma once

#include "common.h"
#include "hittable.h"
#include "ray.h"

class Triangle : public Hittable
{
public:
    Triangle() = default;
    Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, std::shared_ptr<Material> _material, bool double_sided = true)
        : v0{ v0 }
        , v1{ v1 }
        , v2{ v2 }
        , material{ _material }
        , one_sided{ !double_sided }
    {
        normal = Cross(v1 - v0, v2 - v0);
        area = normal.Normalize() / 2.0;

        v0v1 = v1 - v0;
        v0v2 = v2 - v0;
    };

    virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;

public:
    Vec3 v0, v1, v2;
    Vec3 v0v1, v0v2;
    Vec3 normal;
    double area;
    std::shared_ptr<Material> material;
    bool one_sided;
};

static constexpr Vec3 epsilon_offset{ epsilon * 10.0 };

inline bool Triangle::GetAABB(AABB& outAABB) const
{
    outAABB.min = Min(Min(v0, v1), v2) - epsilon_offset;
    outAABB.max = Max(Max(v0, v1), v2) + epsilon_offset;

    return true;
}