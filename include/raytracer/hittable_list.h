#pragma once

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
        objects.push_back(object);
    }

    virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;

    std::vector<std::shared_ptr<Hittable>> objects;
};

bool HittableList::Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const
{
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