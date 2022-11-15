#pragma once

#include "raytracer/hittable_list.h"

bool HittableList::Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const
{
#if USE_BVH
    bool hit_closest = false;

    double t = t_max;

    bvh.RayCast(ray, t_min, t_max, [&](const Ray& _ray, double _t_min, double _t_max, Hittable* _object) -> double {
        bool hit = _object->Hit(ray, _t_min, _t_max, rec);

        if (hit)
        {
            hit_closest = true;
            t = rec.t;
        }

        return t;
    });

    return hit_closest;
#else
    HitRecord tmp;
    bool hit = false;
    double closest = t_max;

    for (const auto& object : objects)
    {
        if (object->Hit(ray, t_min, closest, tmp))
        {
            hit = true;
            closest = tmp.t;
            rec = tmp;
        }
    }

    return hit;
#endif
}

bool HittableList::GetAABB(AABB& outAABB) const
{
    if (objects.empty())
    {
        return false;
    }

    objects[0]->GetAABB(outAABB);

    AABB temp;
    for (int32 i = 1; i < objects.size(); ++i)
    {
        const auto& object = objects[i];

        if (object->GetAABB(temp) == false)
        {
            return false;
        }

        outAABB = Union(outAABB, temp);
    }

    return true;
}
