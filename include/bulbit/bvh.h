#pragma once

#include "bulbit/growable_array.h"
#include "bulbit/parallel.h"
#include "primitive.h"

#include <memory_resource>
#include <span>

namespace bulbit
{

struct BVHPrimitive
{
    BVHPrimitive() = default;
    BVHPrimitive(size_t index, const AABB& aabb)
        : index{ index }
        , aabb{ aabb }
    {
    }

    size_t index;
    AABB aabb;
};

struct BVHNode
{
    void InitLeaf(int32 _offset, int32 _count, const AABB& _aabb)
    {
        offset = _offset;
        count = _count;
        aabb = _aabb;
        child1 = nullptr;
        child2 = nullptr;
    }

    void InitInternal(int32 _axis, BVHNode* _child1, BVHNode* _child2)
    {
        axis = _axis;
        child1 = _child1;
        child2 = _child2;
        aabb = AABB::Union(child1->aabb, child2->aabb);
        count = 0;
    }

    AABB aabb;
    int32 axis, offset, count;

    BVHNode* child1;
    BVHNode* child2;
};

class BVH : public Intersectable
{
public:
    BVH(const std::vector<Primitive*>& primitives);

    virtual AABB GetAABB() const override;
    virtual bool Intersect(Intersection* out_is, const Ray& ray, Float t_min, Float t_max) const override;
    virtual bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const override;

private:
    friend class Scene;

    BVHNode* BuildRecursive(ThreadLocal<Allocator>& thread_allocators,
                            std::span<BVHPrimitive> primitive_span,
                            std::atomic<int32>* total_nodes,
                            std::atomic<int32>* ordered_prims_offset,
                            std::vector<Primitive*>& ordered_prims);

    template <typename T>
    void RayCast(const Ray& r, Float t_min, Float t_max, T* callback) const;

    std::vector<Primitive*> primitives;
    BVHNode* root;
};

template <typename T>
void BVH::RayCast(const Ray& r, Float t_min, Float t_max, T* callback) const
{
    GrowableArray<BVHNode*, 256> stack;
    stack.Emplace(root);

    while (stack.Count() > 0)
    {
        BVHNode* node = stack.Pop();
        if (node == nullptr)
        {
            continue;
        }

        if (node->count > 0)
        {
            for (int32 i = 0; i < node->count; ++i)
            {
                Float t = callback->RayCastCallback(r, t_min, t_max, primitives[node->offset + i]);
                if (t <= t_min)
                {
                    return;
                }
                else
                {
                    // Shorten the ray
                    t_max = t;
                }
            }
        }
        else
        {
            // Ordered traversal
            BVHNode* child1 = node->child1;
            BVHNode* child2 = node->child2;

            Float dist1 = child1->aabb.Intersect(r, t_min, t_max);
            Float dist2 = child2->aabb.Intersect(r, t_min, t_max);

            if (dist2 < dist1)
            {
                std::swap(dist1, dist2);
                std::swap(child1, child2);
            }

            if (dist1 == infinity)
            {
                continue;
            }
            else
            {
                if (dist2 != infinity)
                {
                    stack.Emplace(child2);
                }
                stack.Emplace(child1);
            }
        }
    }
}

} // namespace bulbit