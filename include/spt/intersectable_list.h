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
    virtual ~IntersectableList() = default;
    void Clear();

    virtual bool Intersect(Intersection* out_is, const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual bool IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual bool GetAABB(AABB* out_aabb) const override;
    virtual Vec3 Sample(const Point3& origin) const override;
    virtual f64 EvaluatePDF(const Ray& ray) const override;
    virtual i32 GetSize() const override;
    virtual void Rebuild() override;

    const std::vector<Ref<Intersectable>>& GetObjects() const;
    void Add(const Ref<Intersectable>& object);

    size_t GetCount() const;

private:
    BVH bvh;
    std::vector<Ref<Intersectable>> objects;
};

inline void IntersectableList::Add(const Ref<Intersectable>& object)
{
    AABB aabb;
    object->GetAABB(&aabb);

    bvh.CreateNode(object.get(), aabb);
    objects.push_back(object);
}

inline void IntersectableList::Clear()
{
    bvh.Reset();
    objects.clear();
}

inline bool IntersectableList::Intersect(Intersection* is, const Ray& ray, f64 t_min, f64 t_max) const
{
#if USE_BVH
    return bvh.Intersect(is, ray, t_min, t_max);
#else
    Intersection tmp;
    bool hit = false;
    f64 closest = t_max;

    for (const auto& object : objects)
    {
        if (object->Intersect(&tmp, ray, t_min, closest))
        {
            hit = true;
            closest = tmp.t;
            *is = tmp;
        }
    }

    return hit;
#endif
}

inline bool IntersectableList::IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const
{
#if USE_BVH
    return bvh.IntersectAny(ray, t_min, t_max);
#else
    for (const auto& object : objects)
    {
        if (object->IntersectAny(ray, t_min, t_max))
        {
            return true;
        }
    }

    return false;
#endif
}

inline bool IntersectableList::GetAABB(AABB* out_aabb) const
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

        if (object->GetAABB(&temp) == false)
        {
            return false;
        }

        *out_aabb = Union(*out_aabb, temp);
    }

    return true;
#endif
}

inline Vec3 IntersectableList::Sample(const Point3& origin) const
{
    return bvh.Sample(origin);
}

inline f64 IntersectableList::EvaluatePDF(const Ray& ray) const
{
    return bvh.EvaluatePDF(ray);
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