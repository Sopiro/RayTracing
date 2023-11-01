#include "bulbit/aggregate.h"

namespace bulbit
{

bool Aggregate::Intersect(Intersection* is, const Ray& ray, Float t_min, Float t_max) const
{
    struct Callback
    {
        Intersection* is;
        bool hit_closest;
        Float t;

        Float RayCastCallback(const Ray& ray, Float t_min, Float t_max, Intersectable* object)
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

    is->object = this;
    return callback.hit_closest;
}

bool Aggregate::IntersectAny(const Ray& ray, Float t_min, Float t_max) const
{
    struct Callback
    {
        bool hit_any;

        Float RayCastCallback(const Ray& ray, Float t_min, Float t_max, Intersectable* object)
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

Float Aggregate::EvaluatePDF(const Ray& ray) const
{
    struct Callback
    {
        Float sum;
        Float weight;

        Intersection is;

        Float RayCastCallback(const Ray& ray, Float t_min, Float t_max, Intersectable* object)
        {
            bool hit = object->Intersect(&is, ray, t_min, t_max);

            if (hit)
            {
                sum += ((Primitive*)is.object)->PDFValue(is, ray);
            }

            return t_max;
        }
    } callback;

    callback.sum = 0;

    bvh.RayCast(ray, Ray::epsilon, infinity, &callback);

    return callback.sum / Float(primitives.size());
}

} // namespace bulbit
