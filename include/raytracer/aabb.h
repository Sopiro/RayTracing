#pragma once

#include "common.h"
#include "ray.h"

class AABB
{
public:
    AABB() = default;

    bool Hit(const Ray& r, double t_min, double t_max) const;

    Vec3 min;
    Vec3 max;
};

inline double Area(const AABB& aabb)
{
    return (aabb.max.x - aabb.min.x) * (aabb.max.y - aabb.min.y) * (aabb.max.z - aabb.min.z);
}

inline double Perimeter(const AABB& aabb)
{
    Vec3 w = aabb.max - aabb.min;
    return 2.0 * ((w.x * w.y) + (w.y * w.z) + (w.z * w.x));
}

inline void Fix(AABB& aabb)
{
    auto a = Max(aabb.min, aabb.max);

    Vec3 newMin = Min(aabb.min, aabb.max);
    Vec3 newMax = Max(aabb.min, aabb.max);

    aabb.min = newMin;
    aabb.max = newMax;
}

inline AABB Union(const AABB& b1, const AABB& b2)
{
    Vec3 min = Min(b1.min, b2.min);
    Vec3 max = Max(b1.max, b2.max);

    return AABB{ min, max };
}

inline bool TestPointInsideAABB(const AABB& aabb, const Vec2& point)
{
    if (aabb.min.x > point.x || aabb.max.x < point.x) return false;
    if (aabb.min.y > point.y || aabb.max.y < point.y) return false;

    return true;
}

inline bool TestOverlapAABB(const AABB& a, const AABB& b)
{
    if (a.min.x > b.max.x || a.max.x < b.min.x) return false;
    if (a.min.y > b.max.y || a.max.y < b.min.y) return false;

    return true;
}

inline bool ContainsAABB(const AABB& container, const AABB& testee)
{
    // clang-format off
    return container.min.x <= testee.min.x
        && container.min.y <= testee.min.y
        && container.max.x >= testee.max.x
        && container.max.y >= testee.max.y;
    // clang-format on
}

inline bool AABB::Hit(const Ray& r, double t_min, double t_max) const
{
    for (uint32 axis = 0; axis < 3; ++axis)
    {
        double invD = 1.0f / r.dir[axis];

        double t0 = (min[axis] - r.origin[axis]) * invD;
        double t1 = (max[axis] - r.origin[axis]) * invD;

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