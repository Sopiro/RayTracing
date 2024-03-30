#pragma once

#include "aabb.h"
#include "growable_array.h"
#include "intersectable.h"

namespace bulbit
{

constexpr Vec3 aabb_margin{ 0 };
constexpr Float aabb_multiplier{ 1 };

class Intersectable;

class DynamicBVH
{
public:
    using Data = Intersectable;

    static constexpr inline NodeProxy null_node = -1;

    struct Node
    {
        bool IsLeaf() const
        {
            return child1 == null_node;
        }

        AABB aabb;

        NodeProxy parent;
        NodeProxy child1;
        NodeProxy child2;

        NodeProxy next;
        bool moved;

        Data* data; // user data
    };

    DynamicBVH();

    virtual ~DynamicBVH() noexcept;

    DynamicBVH(const DynamicBVH&) = delete;
    DynamicBVH& operator=(const DynamicBVH&) = delete;

    DynamicBVH(DynamicBVH&&) noexcept;
    DynamicBVH& operator=(DynamicBVH&&) noexcept;

    void Reset();

    NodeProxy PoolNode(Data* data, const AABB& aabb);
    NodeProxy CreateNode(Data* data, const AABB& aabb);
    bool MoveNode(NodeProxy node, AABB aabb, const Vec3& displacement, bool force_move);
    void RemoveNode(NodeProxy node);

    bool TestOverlap(NodeProxy nodeA, NodeProxy nodeB) const;
    const AABB& GetAABB(NodeProxy node) const;
    void ClearMoved(NodeProxy node) const;
    bool WasMoved(NodeProxy node) const;
    Data* GetData(NodeProxy node) const;

    template <typename T>
    void Traverse(T* callback) const;
    template <typename T>
    void Query(const Vec3& point, T* callback) const;
    template <typename T>
    void Query(const AABB& aabb, T* callback) const;
    template <typename T>
    void RayCast(const Ray& r, Float t_min, Float t_max, T* callback) const;

    Float GetTreeCost() const;

    void Rebuild();

private:
    friend class Aggregate;
    friend class Scene;

    NodeProxy root;

    Node* nodes;
    int32 nodeCount;
    int32 nodeCapacity;

    NodeProxy freeList;

    NodeProxy AllocateNode();
    void FreeNode(NodeProxy node);

    NodeProxy InsertLeaf(NodeProxy leaf);
    void RemoveLeaf(NodeProxy leaf);

    void Rotate(NodeProxy node);
    void Swap(NodeProxy node1, NodeProxy node2);

    static Float SAH(const AABB& aabb);
};

inline Float DynamicBVH::SAH(const AABB& aabb)
{
#if 0
    return aabb.GetVolume();
#else
    return aabb.GetSurfaceArea();
#endif
}

inline bool DynamicBVH::TestOverlap(NodeProxy nodeA, NodeProxy nodeB) const
{
    assert(0 <= nodeA && nodeA < nodeCapacity);
    assert(0 <= nodeB && nodeB < nodeCapacity);

    return nodes[nodeA].aabb.TestOverlap(nodes[nodeB].aabb);
}

inline const AABB& DynamicBVH::GetAABB(NodeProxy node) const
{
    assert(0 <= node && node < nodeCapacity);

    return nodes[node].aabb;
}

inline void DynamicBVH::ClearMoved(NodeProxy node) const
{
    assert(0 <= node && node < nodeCapacity);

    nodes[node].moved = false;
}

inline bool DynamicBVH::WasMoved(NodeProxy node) const
{
    assert(0 <= node && node < nodeCapacity);

    return nodes[node].moved;
}

inline DynamicBVH::Data* DynamicBVH::GetData(NodeProxy node) const
{
    assert(0 <= node && node < nodeCapacity);

    return nodes[node].data;
}

inline Float DynamicBVH::GetTreeCost() const
{
    struct
    {
        Float cost = 0;
        void TraverseCallback(const Node* node)
        {
            cost += SAH(node->aabb);
        }
    } temp;

    Traverse(&temp);

    return temp.cost;
}

template <typename T>
void DynamicBVH::Traverse(T* callback) const
{
    if (root == null_node)
    {
        return;
    }

    GrowableArray<NodeProxy, 256> stack;
    stack.Emplace(root);

    while (stack.Count() != 0)
    {
        NodeProxy current = stack.Pop();

        if (!nodes[current].IsLeaf())
        {
            stack.Emplace(nodes[current].child1);
            stack.Emplace(nodes[current].child2);
        }

        const Node* node = nodes + current;
        callback->TraverseCallback(node);
    }
}

template <typename T>
void DynamicBVH::Query(const Vec3& point, T* callback) const
{
    if (root == null_node)
    {
        return;
    }

    GrowableArray<NodeProxy, 256> stack;
    stack.Emplace(root);

    while (stack.Count() != 0)
    {
        NodeProxy current = stack.Pop();

        if (nodes[current].aabb.TestPoint(point) == false)
        {
            continue;
        }

        if (nodes[current].IsLeaf())
        {
            bool proceed = callback->QueryCallback(current, nodes[current].data);
            if (proceed == false)
            {
                return;
            }
        }
        else
        {
            stack.Emplace(nodes[current].child1);
            stack.Emplace(nodes[current].child2);
        }
    }
}

template <typename T>
void DynamicBVH::Query(const AABB& aabb, T* callback) const
{
    if (root == null_node)
    {
        return;
    }

    GrowableArray<NodeProxy, 256> stack;
    stack.Emplace(root);

    while (stack.Count() != 0)
    {
        NodeProxy current = stack.Pop();

        if (nodes[current].aabb.TestOverlap(aabb) == false)
        {
            continue;
        }

        if (nodes[current].IsLeaf())
        {
            bool proceed = callback->QueryCallback(current, nodes[current].data);
            if (proceed == false)
            {
                return;
            }
        }
        else
        {
            stack.Emplace(nodes[current].child1);
            stack.Emplace(nodes[current].child2);
        }
    }
}

template <typename T>
void DynamicBVH::RayCast(const Ray& r, Float t_min, Float t_max, T* callback) const
{
    GrowableArray<int32, 256> stack;
    stack.Emplace(root);

    while (stack.Count() > 0)
    {
        int32 nodeID = stack.Pop();
        if (nodeID == null_node)
        {
            continue;
        }

        const Node* node = nodes + nodeID;

        if (node->IsLeaf())
        {
            Float t = callback->RayCastCallback(r, t_min, t_max, node->data);
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
        else
        {
            // Ordered traversal
            NodeProxy child1 = node->child1;
            NodeProxy child2 = node->child2;

            Float dist1 = nodes[child1].aabb.Intersect(r, t_min, t_max);
            Float dist2 = nodes[child2].aabb.Intersect(r, t_min, t_max);

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