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

    void Add(std::shared_ptr<Hittable> object);
    void Clear();
    void ReBuildBVH();

    virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;

    std::vector<std::shared_ptr<Hittable>> objects;

    BVH bvh;
};

inline void HittableList::Add(std::shared_ptr<Hittable> object)
{
#if USE_BVH
    Hittable* raw = object.get();
    AABB aabb;
    raw->GetAABB(aabb);

    bvh.Insert(raw, aabb);
#endif
    objects.push_back(object);
}

inline void HittableList::Clear()
{
    bvh.Reset();
    objects.clear();
}

inline void HittableList::ReBuildBVH()
{
    bvh.ReBuild();
}