#pragma once

#include "bvh.h"
#include "common.h"
#include "hittable.h"

#define USE_BVH 1

namespace spt
{

class Scene : public Hittable
{
public:
    Scene() = default;

    void Add(std::shared_ptr<Hittable> object);
    void Clear();
    void RebuildBVH();

    virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;
    virtual double EvaluatePDF(const Vec3& origin, const Vec3& dir) const override;
    virtual Vec3 GetRandomDirection(const Vec3& origin) const override;

private:
    BVH bvh;
    std::vector<std::shared_ptr<Hittable>> objects;
};

inline void Scene::Add(std::shared_ptr<Hittable> object)
{
    Hittable* raw = object.get();
    AABB aabb;
    raw->GetAABB(aabb);

    NodeProxy node = bvh.CreateNode(raw, aabb);

    objects.push_back(object);
}

inline void Scene::Clear()
{
    bvh.Reset();
    objects.clear();
}

inline void Scene::RebuildBVH()
{
    bvh.Rebuild();
}

inline double Scene::EvaluatePDF(const Vec3& origin, const Vec3& dir) const
{
    return bvh.EvaluatePDF(origin, dir);
}

inline Vec3 Scene::GetRandomDirection(const Vec3& origin) const
{
    return bvh.GetRandomDirection(origin);
}

} // namespace spt