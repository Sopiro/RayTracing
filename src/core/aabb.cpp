#include "bulbit/aabb.h"

namespace bulbit
{

bool AABB::Intersect(const Ray& ray, Float t_min, Float t_max) const
{
    // https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies/anoptimizedaabbhitmethod
    for (int32 axis = 0; axis < 3; ++axis)
    {
        Float invD = 1 / ray.d[axis];

        Float t0 = (min[axis] - ray.o[axis]) * invD;
        Float t1 = (max[axis] - ray.o[axis]) * invD;

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

} // namespace bulbit
