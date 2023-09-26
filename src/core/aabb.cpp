#include "spt/aabb.h"

namespace spt
{

bool AABB::Intersect(const Ray& ray, f64 t_min, f64 t_max) const
{
    // https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies/anoptimizedaabbhitmethod
    for (i32 axis = 0; axis < 3; ++axis)
    {
        f64 invD = 1.0 / ray.d[axis];

        f64 t0 = (min[axis] - ray.o[axis]) * invD;
        f64 t1 = (max[axis] - ray.o[axis]) * invD;

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

} // namespace spt
