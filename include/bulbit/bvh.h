#pragma once

#include "bulbit/growable_array.h"
#include "bulbit/parallel.h"
#include "primitive.h"

#include <memory_resource>
#include <span>

namespace bulbit
{

struct BVHNode;
struct BVHPrimitive;

class BVH : public Intersectable
{

public:
    using Resource = std::pmr::monotonic_buffer_resource;
    using Allocator = std::pmr::polymorphic_allocator<std::byte>;

    BVH(const std::vector<Ref<Primitive>>& primitives);

    virtual void GetAABB(AABB* out_aabb) const override;
    virtual bool Intersect(Intersection* out_is, const Ray& ray, Float t_min, Float t_max) const override;
    virtual bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const override;

private:
    friend class Scene;

    BVHNode* BuildRecursive(ThreadLocal<Allocator>& thread_allocators,
                            std::span<BVHPrimitive> primitive_span,
                            std::atomic<int32>* total_nodes,
                            std::atomic<int32>* ordered_prims_offset,
                            std::vector<Ref<Primitive>>& ordered_prims);

    template <typename T>
    void RayCast(const Ray& r, Float t_min, Float t_max, T* callback) const;

    std::vector<Ref<Primitive>> primitives;
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
                Float t = callback->RayCastCallback(r, t_min, t_max, primitives[node->offset + i].get());
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