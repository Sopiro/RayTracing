#include "spt/aggregate.h"

namespace spt
{

bool Aggregate::Intersect(Intersection* is, const Ray& ray, f64 t_min, f64 t_max) const
{
    struct Callback
    {
        Intersection* is;
        bool hit_closest;
        f64 t;

        f64 RayCastCallback(const Ray& ray, f64 t_min, f64 t_max, Intersectable* object)
        {
            bool hit = object->Intersect(is, ray, t_min, t_max);

            if (hit)
            {
                hit_closest = true;
                t = is->t;
            }

            // Keep traverse with smaller bounds
            return t;
        }
    } callback;

    callback.is = is;
    callback.hit_closest = false;
    callback.t = t_max;

    bvh.RayCast(ray, t_min, t_max, &callback);

    return callback.hit_closest;
}

bool Aggregate::IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const
{
    struct Callback
    {
        bool hit_any;

        f64 RayCastCallback(const Ray& ray, f64 t_min, f64 t_max, Intersectable* object)
        {
            bool hit = object->IntersectAny(ray, t_min, t_max);

            if (hit)
            {
                hit_any = true;

                // Stop traversal
                return t_min;
            }

            return t_max;
        }
    } callback;

    callback.hit_any = false;

    bvh.RayCast(ray, t_min, t_max, &callback);

    return callback.hit_any;
}

} // namespace spt
