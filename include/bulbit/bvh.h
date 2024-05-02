#pragma once

#include "growable_array.h"
#include "parallel.h"
#include "primitive.h"

namespace bulbit
{

class BVH : public Intersectable
{
public:
    BVH(const std::vector<Primitive*>& primitives);
    ~BVH() noexcept;

    virtual AABB GetAABB() const override;
    virtual bool Intersect(Intersection* out_is, const Ray& ray, Float t_min, Float t_max) const override;
    virtual bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const override;

private:
    friend class Scene;

    struct BVHPrimitive
    {
        BVHPrimitive() = default;
        BVHPrimitive(size_t index, const AABB& aabb);

        size_t index;
        AABB aabb;
    };

    struct BuildNode
    {
        void InitLeaf(int32 offset, int32 count, const AABB& aabb);
        void InitInternal(int32 axis, BuildNode* child1, BuildNode* child2);

        AABB aabb;
        int32 axis, offset, count;

        BuildNode* child1;
        BuildNode* child2;
    };

    struct alignas(32) LinearBVHNode
    {
        AABB aabb;

        union
        {
            int32 primitives_offset;
            int32 child2_offset;
        };

        uint16 primitive_count;
        uint8 axis;
    };

    BuildNode* BuildRecursive(ThreadLocal<Allocator>& thread_allocators,
                              std::span<BVHPrimitive> primitive_span,
                              std::atomic<int32>* total_nodes,
                              std::atomic<int32>* ordered_prims_offset,
                              std::vector<Primitive*>& ordered_prims);

    int32 FlattenBVH(BuildNode* node, int32* offset);

    template <typename T>
    void RayCast(const Ray& r, Float t_min, Float t_max, T* callback) const;

    std::vector<Primitive*> primitives;
    LinearBVHNode* nodes;
};

inline BVH::BVHPrimitive::BVHPrimitive(size_t index, const AABB& aabb)
    : index{ index }
    , aabb{ aabb }
{
}

inline void BVH::BuildNode::InitLeaf(int32 _offset, int32 _count, const AABB& _aabb)
{
    offset = _offset;
    count = _count;
    aabb = _aabb;
    child1 = nullptr;
    child2 = nullptr;
}

inline void BVH::BuildNode::InitInternal(int32 _axis, BuildNode* _child1, BuildNode* _child2)
{
    axis = _axis;
    child1 = _child1;
    child2 = _child2;
    aabb = AABB::Union(child1->aabb, child2->aabb);
    count = 0;
}

template <typename T>
inline void BVH::RayCast(const Ray& r, Float t_min, Float t_max, T* callback) const
{
    const Vec3 inv_dir(1 / r.d.x, 1 / r.d.y, 1 / r.d.z);
    const int32 is_dir_neg[3] = { int32(inv_dir.x < 0), int32(inv_dir.y < 0), int32(inv_dir.z < 0) };

    GrowableArray<int32, 64> stack;
    stack.Emplace(0);

    while (stack.Count() > 0)
    {
        int32 index = stack.Pop();

        if (nodes[index].aabb.TestRay(r.o, t_min, t_max, inv_dir, is_dir_neg))
        {
            if (nodes[index].primitive_count > 0)
            {
                // Leaf node
                for (int32 i = 0; i < nodes[index].primitive_count; ++i)
                {
                    Float t = callback->RayCastCallback(r, t_min, t_max, primitives[nodes[index].primitives_offset + i]);
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
                // Internal node

                // Ordered traversal
                // Put far child on stack first
                int32 child1 = index + 1;
                int32 child2 = nodes[index].child2_offset;

                if (is_dir_neg[nodes[index].axis])
                {
                    stack.Emplace(child1);
                    stack.Emplace(child2);
                }
                else
                {
                    stack.Emplace(child2);
                    stack.Emplace(child1);
                }
            }
        }
    }
}

} // namespace bulbit