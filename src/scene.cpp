#pragma once

#include "raytracer/scene.h"

bool Scene::Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const
{
#if USE_BVH
    return bvh.Hit(ray, t_min, t_max, rec);
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

bool Scene::GetAABB(AABB& outAABB) const
{
#if USE_BVH
    return bvh.GetAABB(outAABB);
#else
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
#endif
}
