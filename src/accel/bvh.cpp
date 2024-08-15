#include "bulbit/bvh.h"
#include "bulbit/intersectable.h"
#include "bulbit/parallel_for.h"

#include <algorithm>

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

    BuildNode* root = BuildRecursive(
        thread_allocators, std::span<BVHPrimitive>(bvh_primitives), &total_nodes, &ordered_prims_offset, ordered_prims
    );

    BulbitAssert(ordered_prims_offset.load() == primitive_count);

    primitives.swap(ordered_prims);

    // Release temporary data
    bvh_primitives.resize(0);
    bvh_primitives.shrink_to_fit();

    nodes = new LinearBVHNode[total_nodes];
    int32 offset = 0;

    // Flatten out to linear BVH representation
    FlattenBVH(root, &offset);

    BulbitAssert(offset == total_nodes);
}

BVH::~BVH() noexcept
{
    delete nodes;
}

BVH::BuildNode* BVH::BuildRecursive(
    ThreadLocal<Allocator>& thread_allocators,
    std::span<BVHPrimitive> primitive_span,
    std::atomic<int32>* total_nodes,
    std::atomic<int32>* ordered_prims_offset,
    std::vector<Primitive*>& ordered_prims
)
{
    Allocator allocator = thread_allocators.Get();
    BuildNode* node = allocator.new_object<BuildNode>();

    total_nodes->fetch_add(1);
    int32 primitive_count = int32(primitive_span.size());

    AABB span_bounds;
    for (const BVHPrimitive& prim : primitive_span)
    {
        span_bounds = AABB::Union(span_bounds, prim.aabb);
    }

    if (span_bounds.GetSurfaceArea() == 0 || primitive_count == 1)
    {
        int32 offset = ordered_prims_offset->fetch_add(primitive_count);
        for (int32 i = 0; i < primitive_count; ++i)
        {
            int32 index = int32(primitive_span[i].index);
            ordered_prims[offset + i] = primitives[index];
        }

        node->InitLeaf(offset, primitive_count, span_bounds);
        return node;
    }

    AABB centroid_bounds;
    for (const BVHPrimitive& prim : primitive_span)
    {
        centroid_bounds = AABB::Union(centroid_bounds, prim.aabb.GetCenter());
    }

    // Get the longest extent
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

    const Float extent = extents[axis];

    // Invalid bounds
    if (extent == 0)
    {
        int32 offset = ordered_prims_offset->fetch_add(primitive_count);
        for (int32 i = 0; i < primitive_count; ++i)
        {
            int32 index = int32(primitive_span[i].index);
            ordered_prims[offset + i] = primitives[index];
        }

        node->InitLeaf(offset, primitive_count, span_bounds);
        return node;
    }

    int32 mid;

#if 0
    // Mid point split
    Float split_pos = (span_bounds.min[axis] + span_bounds.max[axis]) / 2;
    auto mid_iter = std::partition(primitive_span.begin(), primitive_span.end(),
                                   [=](const BVHPrimitive& prim) { return prim.aabb.GetCenter()[axis] < split_pos; });

    mid = mid_iter - primitive_span.begin();

    if (mid_iter == primitive_span.begin() || mid_iter == primitive_span.end())
    {
        mid = primitive_count / 2;
        std::nth_element(
            primitive_span.begin(), primitive_span.begin() + mid, primitive_span.end(),
            [axis](const BVHPrimitive& a, const BVHPrimitive& b) { return a.aabb.GetCenter()[axis] < b.aabb.GetCenter()[axis]; });
    }
#else
    // SAH based construction

    if (primitive_count <= 2)
    {
        mid = primitive_count / 2;
        std::nth_element(
            primitive_span.begin(), primitive_span.begin() + mid, primitive_span.end(),
            [axis](const BVHPrimitive& a, const BVHPrimitive& b) { return a.aabb.GetCenter()[axis] < b.aabb.GetCenter()[axis]; }
        );
    }
    else
    {
        struct BVHSplitBucket
        {
            int count = 0;
            AABB bounds;
        };

        constexpr int32 bucket_size = 12;
        constexpr int32 split_planes = bucket_size - 1;
        BVHSplitBucket buckets[bucket_size];

        // Fill the buckets for
        for (const BVHPrimitive& primitive : primitive_span)
        {
            int32 bucket_index = int32(bucket_size * (primitive.aabb.GetCenter() - centroid_bounds.min)[axis] / extent);
            if (bucket_index == bucket_size)
            {
                bucket_index = bucket_index - 1;
            }

            buckets[bucket_index].count++;
            buckets[bucket_index].bounds = AABB::Union(buckets[bucket_index].bounds, primitive.aabb);
        }

        AABB left_bound, right_bound;
        int32 left_count[split_planes], right_count[split_planes];
        Float left_area[split_planes], right_area[split_planes];
        int32 left_sum = 0, right_sum = 0;

        for (int32 i = 0; i < split_planes; ++i)
        {
            left_sum += buckets[i].count;
            left_count[i] = left_sum;
            left_bound = AABB::Union(left_bound, buckets[i].bounds);
            left_area[i] = left_bound.GetSurfaceArea();

            right_sum += buckets[split_planes - i].count;
            right_count[split_planes - 1 - i] = right_sum;
            right_bound = AABB::Union(right_bound, buckets[split_planes - i].bounds);
            right_area[split_planes - 1 - i] = right_bound.GetSurfaceArea();
        }

        int32 min_cost_split_bucket = -1;
        Float min_cost = infinity;
        for (int32 i = 0; i < split_planes; ++i)
        {
            Float cost = left_count[i] * left_area[i] + right_count[i] * right_area[i];
            if (cost < min_cost)
            {
                min_cost = cost;
                min_cost_split_bucket = i;
            }
        }

        constexpr Float traverse_cost = 0.5f;
        min_cost = traverse_cost + min_cost / span_bounds.GetSurfaceArea();

        const Float direct_leaf_cost = Float(primitive_count);

        if (min_cost < direct_leaf_cost)
        {
            auto mid_iter = std::partition(primitive_span.begin(), primitive_span.end(), [=](const BVHPrimitive& prim) {
                int32 bucket_index = int32(bucket_size * (prim.aabb.GetCenter() - centroid_bounds.min)[axis] / extent);
                if (bucket_index == bucket_size)
                {
                    bucket_index = bucket_size - 1;
                }

                return bucket_index <= min_cost_split_bucket;
            });

            mid = int32(mid_iter - primitive_span.begin());
        }
        else
        {
            int32 offset = ordered_prims_offset->fetch_add(primitive_count);
            for (int32 i = 0; i < primitive_count; ++i)
            {
                int32 index = int32(primitive_span[i].index);
                ordered_prims[offset + i] = primitives[index];
            }

            node->InitLeaf(offset, primitive_count, span_bounds);
            return node;
        }
    }
#endif

    BuildNode* child1;
    BuildNode* child2;

    if (primitive_count > 64 * 1024)
    {
        ParallelFor(0, 2, [&](int i) {
            if (i == 0)
            {
                child1 = BuildRecursive(
                    thread_allocators, primitive_span.subspan(0, mid), total_nodes, ordered_prims_offset, ordered_prims
                );
            }
            else
            {
                child2 = BuildRecursive(
                    thread_allocators, primitive_span.subspan(mid), total_nodes, ordered_prims_offset, ordered_prims
                );
            }
        });
    }
    else
    {
        child1 =
            BuildRecursive(thread_allocators, primitive_span.subspan(0, mid), total_nodes, ordered_prims_offset, ordered_prims);
        child2 = BuildRecursive(thread_allocators, primitive_span.subspan(mid), total_nodes, ordered_prims_offset, ordered_prims);
    }

    node->InitInternal(axis, child1, child2);

    return node;
}

int32 BVH::FlattenBVH(BuildNode* node, int32* offset)
{
    LinearBVHNode* linear_node = &nodes[*offset];
    linear_node->aabb = node->aabb;

    int32 node_offset = (*offset)++;
    if (node->count > 0)
    {
        // Leaf node
        linear_node->primitives_offset = node->offset;
        linear_node->primitive_count = uint16(node->count);
    }
    else
    {
        // Internal node
        linear_node->axis = uint8(node->axis);
        linear_node->primitive_count = 0;

        // Order matters!
        FlattenBVH(node->child1, offset);
        linear_node->child2_offset = FlattenBVH(node->child2, offset);
    }

    return node_offset;
}

bool BVH::Intersect(Intersection* isect, const Ray& ray, Float t_min, Float t_max) const
{
    struct Callback
    {
        Intersection* isect;
        bool hit_closest;
        Float t;

        Float RayCastCallback(const Ray& ray, Float t_min, Float t_max, Intersectable* object)
        {
            bool hit = object->Intersect(isect, ray, t_min, t_max);

            if (hit)
            {
                BulbitAssert(isect->t <= t);
                hit_closest = true;
                t = isect->t;
            }

            // Keep traverse with smaller bounds
            return t;
        }
    } callback;

    callback.isect = isect;
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

AABB BVH::GetAABB() const
{
    return nodes[0].aabb;
}

} // namespace bulbit