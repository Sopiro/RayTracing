#pragma once

#include "ray.h"

namespace spt
{

struct AABB
{
    Float GetVolume() const;
    Float GetSurfaceArea() const;

    bool Contains(const AABB& other) const;
    bool TestPoint(const Vec3& point) const;
    bool TestOverlap(const AABB& other) const;
    bool Intersect(const Ray& ray, Float t_min, Float t_max) const;

    Vec3 min, max;

    static AABB Union(const AABB& b1, const AABB& b2);
};

inline Float AABB::GetVolume() const
{
    return (max.x - min.x) * (max.y - min.y) * (max.z - min.z);
}

inline Float AABB::GetSurfaceArea() const
{
    Vec3 w = max - min;
    return 2 * ((w.x * w.y) + (w.y * w.z) + (w.z * w.x));
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

inline AABB AABB::Union(const AABB& aabb1, const AABB& aabb2)
{
    Vec3 min = Min(aabb1.min, aabb2.min);
    Vec3 max = Max(aabb1.max, aabb2.max);

    return AABB{ min, max };
}

} // namespace spt