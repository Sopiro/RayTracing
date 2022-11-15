#pragma once

#include "aabb.h"
#include "common.h"
#include "growable_array.h"

#define nullNode (-1)

inline double SAH(const AABB& aabb)
{
#if 0
    return Area(aabb);
#else
    return Perimeter(aabb);
#endif
}

class Hittable;

struct Node
{
    uint32 id;
    AABB aabb;
    bool isLeaf;

    int32 parent;
    int32 child1;
    int32 child2;

    Hittable* body;
    int32 next;
};

class BVH
{
public:
    BVH();
    ~BVH() noexcept;

    BVH(const BVH&) noexcept = delete;
    BVH& operator=(const BVH&) noexcept = delete;

    BVH(BVH&&) noexcept = delete;
    BVH& operator=(BVH&&) noexcept = delete;

    int32 Add(Hittable* object, const AABB& aabb);
    void Build();
    double ComputeCost() const;

    bool IsBuilt() const;

    void RayCast(const Ray& r,
                 double t_min,
                 double t_max,
                 const std::function<double(const Ray& r, double t_min, double t_max, Hittable*)>& callback) const;

private:
    uint32 nodeID = 0;

    Node* nodes;
    int32 root;

    int32 nodeCount;
    int32 nodeCapacity;

    int32 freeList;

    int32 AllocateNode();
    void FreeNode(int32 node);
};

inline bool BVH::IsBuilt() const
{
    return root != nullNode;
}

inline void BVH::RayCast(const Ray& r,
                         double t_min,
                         double t_max,
                         const std::function<double(const Ray& r, double t_min, double t_max, Hittable*)>& callback) const
{
    Vec3 p1 = r.At(t_min);
    Vec3 p2 = r.At(t_max);
    double t = t_max;

    Vec3 d = p2 - p1;
    assert(d.Length2() > 0.0);
    d.Normalize();

    AABB rayAABB;
    rayAABB.min = Min(p1, p2);
    rayAABB.max = Max(p1, p2);

    GrowableArray<int32, 256> stack;
    stack.Emplace(root);

    while (stack.Count() > 0)
    {
        int32 nodeID = stack.Pop();
        if (nodeID == nullNode)
        {
            continue;
        }

        const Node* node = nodes + nodeID;
        if (TestOverlapAABB(node->aabb, rayAABB) == false)
        {
            continue;
        }

        if (node->aabb.Hit(r, t_min, t) == false)
        {
            continue;
        }

        if (node->isLeaf)
        {
            double value = callback(r, t_min, t, node->body);
            if (value <= t_min)
            {
                return;
            }
            else
            {
                // Update ray AABB
                t = value;
                Vec3 newEnd = r.At(t);

                rayAABB.min = Min(p1, newEnd);
                rayAABB.max = Max(p1, newEnd);
            }
        }
        else
        {
            stack.Emplace(node->child1);
            stack.Emplace(node->child2);
        }
    }
}