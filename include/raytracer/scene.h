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
    virtual double PDFValue(const Vec3& v, const HitRecord& rec) const override;
    virtual Vec3 Random(const Vec3& origin) const override;

private:
    BVH bvh;
    std::vector<std::shared_ptr<Hittable>> objects;
};

inline void Scene::Add(std::shared_ptr<Hittable> object)
{
#if USE_BVH
    Hittable* raw = object.get();
    AABB aabb;
    raw->GetAABB(aabb);

    NodeProxy node = bvh.CreateNode(raw, aabb);
#endif

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

inline double Scene::PDFValue(const Vec3& v, const HitRecord& rec) const
{
    return bvh.PDFValue(v, rec);
}

inline Vec3 Scene::Random(const Vec3& origin) const
{
    return bvh.Random(origin);
}

} // namespace spt