#include "bulbit/aabb.h"

namespace bulbit
{

// https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies/anoptimizedaabbhitmethod
bool AABB::TestRay(const Ray& ray, Float t_min, Float t_max) const
{
    for (int32 axis = 0; axis < 3; ++axis)
    {
        Float invD = 1 / ray.d[axis];
        Float origin = ray.o[axis];

        Float t0 = (min[axis] - origin) * invD;
        Float t1 = (max[axis] - origin) * invD;

        if (invD < 0)
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

// https://www.pbr-book.org/4ed/Shapes/Basic_Shape_Interface#Bounds3::IntersectP
bool AABB::TestRay(Vec3 o, Float t_min, Float t_max, Vec3 inv_dir, const int is_neg_dir[3]) const
{
    const AABB& aabb = *this;

    // Slab test for x component
    Float t_min_x = (aabb[is_neg_dir[0]].x - o.x) * inv_dir.x;
    Float t_max_x = (aabb[1 - is_neg_dir[0]].x - o.x) * inv_dir.x;

    if (t_min > t_max_x || t_max < t_min_x)
    {
        return false;
    }

    if (t_min_x > t_min) t_min = t_min_x;
    if (t_max_x < t_max) t_max = t_max_x;

    // Slab test for y component
    Float t_min_y = (aabb[is_neg_dir[1]].y - o.y) * inv_dir.y;
    Float t_max_y = (aabb[1 - is_neg_dir[1]].y - o.y) * inv_dir.y;

    if (t_min > t_max_y || t_max < t_min_y)
    {
        return false;
    }

    if (t_min_y > t_min) t_min = t_min_y;
    if (t_max_y < t_max) t_max = t_max_y;

    Float t_min_z = (aabb[is_neg_dir[2]].z - o.z) * inv_dir.z;
    Float t_max_z = (aabb[1 - is_neg_dir[2]].z - o.z) * inv_dir.z;

    if (t_min > t_max_z || t_max < t_min_z)
    {
        return false;
    }

    // if (t_max_z > t_min) t_min = t_max_z;
    // if (t_max_z < t_max) t_max = t_max_z;

    return true;
}

Float AABB::Intersect(const Ray& ray, Float t_min, Float t_max) const
{
    for (int32 axis = 0; axis < 3; ++axis)
    {
        Float invD = 1 / ray.d[axis];
        Float origin = ray.o[axis];

        Float t0 = (min[axis] - origin) * invD;
        Float t1 = (max[axis] - origin) * invD;

        if (invD < 0)
        {
            std::swap(t0, t1);
        }

        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;

        if (t_max <= t_min)
        {
            return infinity;
        }
    }

    return t_min;
}

} // namespace bulbit
