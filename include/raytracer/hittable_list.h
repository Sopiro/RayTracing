#pragma once

#include "bvh.h"
#include "common.h"
#include "hittable.h"

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
        Hittable* raw = object.get();
        AABB aabb;
        raw->GetAABB(aabb);

        bvh.Add(raw, aabb);
        objects.push_back(object);
    }

    void BuildBVH()
    {
        if (bvh.IsBuilt() == false)
        {
            bvh.Build();
        }
    }

    virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;

    std::vector<std::shared_ptr<Hittable>> objects;

    BVH bvh;
};

bool HittableList::Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const
{
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