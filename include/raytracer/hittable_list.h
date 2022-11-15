#pragma once

#include "bvh.h"
#include "common.h"
#include "hittable.h"

#define USE_BVH 1

class HittableList : public Hittable
{
public:
    HittableList() = default;
    HittableList(std::shared_ptr<Hittable> object)
    {
        Add(object);
    }

    void Clear()
    {
        objects.clear();
    }

    void Add(std::shared_ptr<Hittable> object)
    {
#if USE_BVH
        Hittable* raw = object.get();
        AABB aabb;
        raw->GetAABB(aabb);

        bvh.Insert(raw, aabb);
#endif
        objects.push_back(object);
    }

    void ReBuildBVH()
    {
        if (bvh.IsBuilt() == false)
        {
            bvh.ReBuild();
        }
    }

    virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;

    std::vector<std::shared_ptr<Hittable>> objects;

    BVH bvh;
};

bool HittableList::Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const
{
#if USE_BVH
    assert(bvh.IsBuilt());

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