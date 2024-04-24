#include "bulbit/bvh.h"
#include "bulbit/intersectable.h"
#include "bulbit/parallel_for.h"

namespace bulbit
{

BVH::BVH(const std::vector<Primitive*>& _primitives)
    : primitives{ std::move(_primitives) }
{
    size_t primitive_count = primitives.size();
    std::vector<BVHPrimitive> bvh_primitives(primitive_count);
    for (size_t i = 0; i < primitive_count; ++i)
    {
        bvh_primitives[i] = BVHPrimitive(i, primitives[i]->GetAABB());
    }

    std::vector<Primitive*> ordered_prims(primitive_count);

    std::atomic<int32> total_nodes(0);
    std::atomic<int32> ordered_prims_offset(0);

    std::vector<std::unique_ptr<Resource>> thread_buffer_resources;
    ThreadLocal<Allocator> thread_allocators([&thread_buffer_resources]() {
        thread_buffer_resources.push_back(std::make_unique<Resource>());
        Resource* ptr = thread_buffer_resources.back().get();
        return Allocator(ptr);
    });

    BVHNode* root = BuildRecursive(thread_allocators, std::span<BVHPrimitive>(bvh_primitives), &total_nodes,
                                   &ordered_prims_offset, ordered_prims);

    assert(ordered_prims_offset.load() == primitive_count);

    primitives.swap(ordered_prims);

    // Release temporary primitives
    bvh_primitives.resize(0);
    bvh_primitives.shrink_to_fit();

    nodes = std::make_unique<LinearBVHNode[]>(total_nodes);
    int32 offset = 0;

    // Flatten out to linear BVH representation
    FlattenBVH(root, &offset);

    assert(offset == total_nodes);
}

BVHNode* BVH::BuildRecursive(ThreadLocal<Allocator>& thread_allocators,
                             std::span<BVHPrimitive> primitive_span,
                             std::atomic<int32>* total_nodes,
                             std::atomic<int32>* ordered_prims_offset,
                             std::vector<Primitive*>& ordered_prims)
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

        if (primitive_count > 64 * 1024)
        {
            ParallelFor(0, 2, [&](int i) {
                if (i == 0)
                {
                    child1 = BuildRecursive(thread_allocators, primitive_span.subspan(0, mid), total_nodes, ordered_prims_offset,
                                            ordered_prims);
                }
                else
                {
                    child2 = BuildRecursive(thread_allocators, primitive_span.subspan(mid), total_nodes, ordered_prims_offset,
                                            ordered_prims);
                }
            });
        }
        else
        {
            child1 = BuildRecursive(thread_allocators, primitive_span.subspan(0, mid), total_nodes, ordered_prims_offset,
                                    ordered_prims);
            child2 =
                BuildRecursive(thread_allocators, primitive_span.subspan(mid), total_nodes, ordered_prims_offset, ordered_prims);
        }

        node->InitInternal(axis, child1, child2);
    }

    return node;
}

int32 BVH::FlattenBVH(BVHNode* node, int32* offset)
{
    LinearBVHNode* linear_node = &nodes[*offset];
    linear_node->aabb = node->aabb;

    int32 node_offset = (*offset)++;
    if (node->count > 0)
    {
        // Leaf node
        linear_node->primitives_offset = node->offset;
        linear_node->primitive_count = node->count;
    }
    else
    {
        // Internal node
        linear_node->axis = node->axis;
        linear_node->primitive_count = 0;

        // Order matters!
        FlattenBVH(node->child1, offset);
        linear_node->child2_offset = FlattenBVH(node->child2, offset);
    }

    return node_offset;
}

AABB BVH::GetAABB() const
{
    return nodes[0].aabb;
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