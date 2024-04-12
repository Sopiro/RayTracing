#include "bulbit/bvh.h"
#include "bulbit/intersectable.h"
#include "bulbit/parallel_for.h"

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

BVH::Resource resource;
BVH::Allocator allocator(&resource);
std::vector<std::unique_ptr<BVH::Resource>> thread_buffer_resources;

BVH::BVH(const std::vector<Ref<Primitive>>& _primitives)
    : primitives{ std::move(_primitives) }
{
    size_t primitive_count = primitives.size();
    std::vector<BVHPrimitive> bvh_primitives(primitive_count);
    for (size_t i = 0; i < primitive_count; ++i)
    {
        AABB aabb;
        primitives[i]->GetAABB(&aabb);
        bvh_primitives[i] = BVHPrimitive(i, aabb);
    }

    std::vector<Ref<Primitive>> ordered_prims(primitive_count);

    std::atomic<int32> total_nodes(0);
    std::atomic<int32> ordered_prims_offset(0);

    ThreadLocal<Allocator> thread_allocators([]() {
        thread_buffer_resources.push_back(std::make_unique<Resource>());
        Resource* ptr = thread_buffer_resources.back().get();
        return Allocator(ptr);
    });

    root = BuildRecursive(thread_allocators, std::span<BVHPrimitive>(bvh_primitives), &total_nodes, &ordered_prims_offset,
                          ordered_prims);

    assert(ordered_prims_offset.load() == primitive_count);

    primitives.swap(ordered_prims);
}

BVHNode* BVH::BuildRecursive(ThreadLocal<Allocator>& thread_allocators,
                             std::span<BVHPrimitive> primitive_span,
                             std::atomic<int32>* total_nodes,
                             std::atomic<int32>* ordered_prims_offset,
                             std::vector<Ref<Primitive>>& ordered_prims)
{
    Allocator allocator = thread_allocators.Get();
    BVHNode* node = allocator.new_object<BVHNode>();

    total_nodes->fetch_add(1);
    int32 primitive_count = primitive_span.size();

    AABB span_bounds;
    for (const BVHPrimitive& prim : primitive_span)
    {
        span_bounds = AABB::Union(span_bounds, prim.aabb);
    }

    if (span_bounds.GetSurfaceArea() == 0 || primitive_count == 1)
    {
        int32 offset = ordered_prims_offset->fetch_add(primitive_count);
        for (size_t i = 0; i < primitive_count; ++i)
        {
            int32 index = primitive_span[i].index;
            ordered_prims[offset + i] = primitives[index];
        }

        node->InitLeaf(offset, primitive_count, span_bounds);
        return node;
    }
    else
    {
        AABB centroid_bounds;
        for (const BVHPrimitive& prim : primitive_span)
        {
            centroid_bounds = AABB::Union(centroid_bounds, prim.aabb.GetCenter());
        }

        // Find longest axis
        int32 axis = 0;
        Vec3 extents = centroid_bounds.GetExtents();
        if (extents.y > extents.x)
        {
            axis = 1;
        }
        if (extents.z > extents[axis])
        {
            axis = 2;
        }

        if (extents[axis] == 0)
        {
            int32 offset = ordered_prims_offset->fetch_add(primitive_count);
            for (size_t i = 0; i < primitive_count; ++i)
            {
                int32 index = primitive_span[i].index;
                ordered_prims[offset + i] = primitives[index];
            }

            node->InitLeaf(offset, primitive_count, span_bounds);
            return node;
        }

        Float split_pos = (span_bounds.min[axis] + span_bounds.max[axis]) / 2;
        auto mid_iter = std::partition(primitive_span.begin(), primitive_span.end(),
                                       [=](const BVHPrimitive& prim) { return prim.aabb.GetCenter()[axis] < split_pos; });

        int32 mid = mid_iter - primitive_span.begin();

        if (mid_iter == primitive_span.begin() || mid_iter == primitive_span.end())
        {
            mid = primitive_count / 2;
            std::nth_element(primitive_span.begin(), primitive_span.begin() + mid, primitive_span.end(),
                             [axis](const BVHPrimitive& a, const BVHPrimitive& b) {
                                 return a.aabb.GetCenter()[axis] < b.aabb.GetCenter()[axis];
                             });
        }

        BVHNode* child1;
        BVHNode* child2;

        std::span<BVHPrimitive> span1 = primitive_span.subspan(0, mid);
        std::span<BVHPrimitive> span2 = primitive_span.subspan(mid);

        if (primitive_count > 64 * 1024)
        {
            ParallelFor(0, 2, [&](int i) {
                if (i == 0)
                {
                    child1 = BuildRecursive(thread_allocators, span1, total_nodes, ordered_prims_offset, ordered_prims);
                }
                else
                {
                    child2 = BuildRecursive(thread_allocators, span2, total_nodes, ordered_prims_offset, ordered_prims);
                }
            });
        }
        else
        {
            child1 = BuildRecursive(thread_allocators, span1, total_nodes, ordered_prims_offset, ordered_prims);
            child2 = BuildRecursive(thread_allocators, span2, total_nodes, ordered_prims_offset, ordered_prims);
        }

        node->InitInternal(axis, child1, child2);
    }

    return node;
}

void BVH::GetAABB(AABB* out_aabb) const
{
    *out_aabb = root->aabb;
}

bool BVH::Intersect(Intersection* out_is, const Ray& ray, Float t_min, Float t_max) const
{
    struct Callback
    {
        Intersection* is;
        bool hit_closest;
        Float t;

        Float RayCastCallback(const Ray& ray, Float t_min, Float t_max, Intersectable* object)
        {
            bool hit = object->Intersect(is, ray, t_min, t_max);

            if (hit)
            {
                hit_closest = true;
                t = std::min(t, is->t);
            }

            // Keep traverse with smaller bounds
            return t;
        }
    } callback;

    callback.is = out_is;
    callback.hit_closest = false;
    callback.t = t_max;

    RayCast(ray, t_min, t_max, &callback);

    return callback.hit_closest;
}

bool BVH::IntersectAny(const Ray& ray, Float t_min, Float t_max) const
{
    struct Callback
    {
        bool hit_any;

        Float RayCastCallback(const Ray& ray, Float t_min, Float t_max, Intersectable* object)
        {
            bool hit = object->IntersectAny(ray, t_min, t_max);

            if (hit)
            {
                hit_any = true;

                // Stop traversal
                return t_min;
            }

            return t_max;
        }
    } callback;

    callback.hit_any = false;

    RayCast(ray, t_min, t_max, &callback);

    return callback.hit_any;
}

} // namespace bulbit