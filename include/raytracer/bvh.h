#pragma once

#include "aabb.h"
#include "common.h"
#include "growable_array.h"

#define nullNode (-1)

inline Real SAH(const AABB& aabb)
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

    int32 Insert(Hittable* body, const AABB& aabb);
    void Remove(Hittable* body);
    void Rotate(int32 node);
    int32 Add(Hittable* object, const AABB& aabb);
    void Reset();

    void Rebuild();
    Real ComputeCost() const;

    void RayCast(const Ray& r,
                 Real t_min,
                 Real t_max,
                 const std::function<Real(const Ray&, Real, Real, Hittable*)>& callback) const;

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
