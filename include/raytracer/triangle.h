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

// Möller-Trumbore algorithm
bool Triangle::Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const
{
    Vec3 d = ray.dir.Normalized();
    Vec3 pvec = Cross(d, v0v2);

    double det = Dot(v0v1, pvec);

    bool backface = det < DBL_EPSILON;
    if (one_sided && backface)
    {
        return false;
    }

    // Ray and triangle are parallel
    if (Abs(det) < DBL_EPSILON)
    {
        return false;
    }

    double invDet = 1.0 / det;

    Vec3 tvec = ray.origin - v0;
    double u = Dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1)
    {
        return false;
    }

    Vec3 qvec = Cross(tvec, v0v1);
    double v = Dot(d, qvec) * invDet;
    if (v < 0 || u + v > 1)
    {
        return false;
    }

    double t = Dot(v0v2, qvec) * invDet;

    if (t < t_min || t > t_max)
    {
        return false;
    }

    rec.mat = material;
    rec.t = t;
    rec.p = ray.At(rec.t);
    rec.SetFaceNormal(ray, normal);
    rec.uv.Set(u, v);

    return true;
}

static constexpr Vec3 min_offset{ DBL_EPSILON };

bool Triangle::GetAABB(AABB& outAABB) const
{
    outAABB.min = Min(Min(v0, v1), v2) - min_offset;
    outAABB.max = Max(Max(v0, v1), v2) + min_offset;

    return true;
}