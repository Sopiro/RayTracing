#pragma once

#include "common.h"
#include "ray.h"

namespace spt
{

struct AABB
{
    double GetArea() const;
    double GetPerimater() const;
    bool Contains(const AABB& other) const;
    bool TestPoint(const Vec3& point) const;
    bool TestOverlap(const AABB& other) const;
    bool Hit(const Ray& ray, double t_min, double t_max) const;

    Vec3 min, max;
};

inline double AABB::GetArea() const
{
    return (max.x - min.x) * (max.y - min.y) * (max.z - min.z);
}

inline double AABB::GetPerimater() const
{
    Vec3 w = max - min;
    return 2.0 * ((w.x * w.y) + (w.y * w.z) + (w.z * w.x));
}

inline bool AABB::Contains(const AABB& other) const
{
    return min.x <= other.min.x && min.y <= other.min.y && max.x >= other.max.x && max.y >= other.max.y;
}

inline bool AABB::TestPoint(const Vec3& point) const
{
    if (min.x > point.x || max.x < point.x) return false;
    if (min.y > point.y || max.y < point.y) return false;
    if (min.z > point.z || max.z < point.z) return false;

    return true;
}

inline bool AABB::TestOverlap(const AABB& other) const
{
    if (min.x > other.max.x || max.x < other.min.x) return false;
    if (min.y > other.max.y || max.y < other.min.y) return false;
    if (min.z > other.max.z || max.z < other.min.z) return false;

    return true;
}

inline bool AABB::Hit(const Ray& ray, double t_min, double t_max) const
{
    // https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies/anoptimizedaabbhitmethod
    for (int32 axis = 0; axis < 3; ++axis)
    {
        double invD = 1.0 / ray.dir[axis];

        double t0 = (min[axis] - ray.origin[axis]) * invD;
        double t1 = (max[axis] - ray.origin[axis]) * invD;

        if (invD < 0.0)
        {
            std::swap(t0, t1);
        }

        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;

        if (t_max <= t_min)
        {
            return false;
        }
    }

    return true;
}

inline AABB Union(const AABB& b1, const AABB& b2)
{
    Vec3 min = Min(b1.min, b2.min);
    Vec3 max = Max(b1.max, b2.max);

    return AABB{ min, max };
}

} // namespace spt