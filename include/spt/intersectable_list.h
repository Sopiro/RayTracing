#pragma once

#include "bvh.h"
#include "common.h"
#include "intersectable.h"

#define USE_BVH 1

namespace spt
{

class IntersectableList : public Intersectable
{
public:
    IntersectableList() = default;

    void Add(const Ref<Intersectable>& object);
    void Clear();

    virtual bool Intersect(const Ray& ray, f64 t_min, f64 t_max, Intersection& is) const override;
    virtual bool GetAABB(AABB& out_aabb) const override;
    virtual f64 EvaluatePDF(const Ray& ray) const override;
    virtual Vec3 GetRandomDirection(const Point3& origin) const override;
    virtual i32 GetSize() const override;
    virtual void Rebuild() override;

    const std::vector<Ref<Intersectable>>& GetObjects() const;
    size_t GetCount() const;

private:
    BVH bvh;
    std::vector<Ref<Intersectable>> objects;
};

inline void IntersectableList::Add(const Ref<Intersectable>& object)
{
    AABB aabb;
    object->GetAABB(aabb);

    bvh.CreateNode(object.get(), aabb);
    objects.push_back(object);
}

inline void IntersectableList::Clear()
{
    bvh.Reset();
    objects.clear();
}

inline bool IntersectableList::Intersect(const Ray& ray, f64 t_min, f64 t_max, Intersection& is) const
{
#if USE_BVH
    return bvh.Intersect(ray, t_min, t_max, is);
#else
    HitRecord tmp;
    bool hit = false;
    f64 closest = t_max;

    for (const auto& object : objects)
    {
        if (object->Hit(ray, t_min, closest, tmp))
        {
            hit = true;
            closest = tmp.t;
            is = tmp;
        }
    }

    return hit;
#endif
}

inline bool IntersectableList::GetAABB(AABB& out_aabb) const
{
#if USE_BVH
    return bvh.GetAABB(out_aabb);
#else
    if (objects.empty())
    {
        return false;
    }

    objects[0]->GetAABB(out_aabb);

    AABB temp;
    for (i32 i = 1; i < objects.size(); ++i)
    {
        const auto& object = objects[i];

        if (object->GetAABB(temp) == false)
        {
            return false;
        }

        out_aabb = Union(out_aabb, temp);
    }

    return true;
#endif
}

inline f64 IntersectableList::EvaluatePDF(const Ray& ray) const
{
    return bvh.EvaluatePDF(ray);
}

inline Vec3 IntersectableList::GetRandomDirection(const Point3& origin) const
{
    return bvh.GetRandomDirection(origin);
}

inline i32 IntersectableList::GetSize() const
{
    return bvh.GetSize();
}

inline void IntersectableList::Rebuild()
{
    bvh.Rebuild();

    struct Callback
    {
        void TraverseCallback(const Node* node)
        {
            if (node->data)
            {
                node->data->Rebuild();
            }
        }
    } callback;

    bvh.Traverse(&callback);
}

inline const std::vector<Ref<Intersectable>>& IntersectableList::GetObjects() const
{
    return objects;
}

inline size_t IntersectableList::GetCount() const
{
    return objects.size();
}

} // namespace spt