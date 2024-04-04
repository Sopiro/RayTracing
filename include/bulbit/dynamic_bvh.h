#pragma once

#include "aabb.h"
#include "growable_array.h"
#include "primitive.h"

namespace bulbit
{

using NodeIndex = int32;

class DynamicBVH
{
public:
    static constexpr inline NodeIndex null_node = -1;

    struct Node
    {
        bool IsLeaf() const
        {
            return child1 == null_node;
        }

        AABB aabb;

        NodeIndex parent;
        NodeIndex child1;
        NodeIndex child2;

        NodeIndex next;
        bool moved;

        Primitive* primitive;
    };

    DynamicBVH();

    virtual ~DynamicBVH() noexcept;

    DynamicBVH(const DynamicBVH&) = delete;
    DynamicBVH& operator=(const DynamicBVH&) = delete;

    DynamicBVH(DynamicBVH&&) noexcept;
    DynamicBVH& operator=(DynamicBVH&&) noexcept;

    void Reset();

    NodeIndex PoolNode(Primitive* data, const AABB& aabb);
    NodeIndex CreateNode(Primitive* data, const AABB& aabb);
    bool MoveNode(NodeIndex node, AABB aabb, const Vec3& displacement, bool force_move);
    void RemoveNode(NodeIndex node);

    bool TestOverlap(NodeIndex nodeA, NodeIndex nodeB) const;
    const AABB& GetAABB(NodeIndex node) const;
    void ClearMoved(NodeIndex node) const;
    bool WasMoved(NodeIndex node) const;
    Primitive* GetData(NodeIndex node) const;

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
    friend class Scene;

    NodeIndex root;

    Node* nodes;
    int32 nodeCount;
    int32 nodeCapacity;

    NodeIndex freeList;

    NodeIndex AllocateNode();
    void FreeNode(NodeIndex node);

    NodeIndex InsertLeaf(NodeIndex leaf);
    void RemoveLeaf(NodeIndex leaf);

    void Rotate(NodeIndex node);
    void Swap(NodeIndex node1, NodeIndex node2);

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

inline bool DynamicBVH::TestOverlap(NodeIndex nodeA, NodeIndex nodeB) const
{
    assert(0 <= nodeA && nodeA < nodeCapacity);
    assert(0 <= nodeB && nodeB < nodeCapacity);

    return nodes[nodeA].aabb.TestOverlap(nodes[nodeB].aabb);
}

inline const AABB& DynamicBVH::GetAABB(NodeIndex node) const
{
    assert(0 <= node && node < nodeCapacity);

    return nodes[node].aabb;
}

inline void DynamicBVH::ClearMoved(NodeIndex node) const
{
    assert(0 <= node && node < nodeCapacity);

    nodes[node].moved = false;
}

inline bool DynamicBVH::WasMoved(NodeIndex node) const
{
    assert(0 <= node && node < nodeCapacity);

    return nodes[node].moved;
}

inline Primitive* DynamicBVH::GetData(NodeIndex node) const
{
    assert(0 <= node && node < nodeCapacity);

    return nodes[node].primitive;
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

    GrowableArray<NodeIndex, 256> stack;
    stack.Emplace(root);

    while (stack.Count() != 0)
    {
        NodeIndex current = stack.Pop();

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

    GrowableArray<NodeIndex, 256> stack;
    stack.Emplace(root);

    while (stack.Count() != 0)
    {
        NodeIndex current = stack.Pop();

        if (nodes[current].aabb.TestPoint(point) == false)
        {
            continue;
        }

        if (nodes[current].IsLeaf())
        {
            bool proceed = callback->QueryCallback(current, nodes[current].primitive);
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

    GrowableArray<NodeIndex, 256> stack;
    stack.Emplace(root);

    while (stack.Count() != 0)
    {
        NodeIndex current = stack.Pop();

        if (nodes[current].aabb.TestOverlap(aabb) == false)
        {
            continue;
        }

        if (nodes[current].IsLeaf())
        {
            bool proceed = callback->QueryCallback(current, nodes[current].primitive);
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
            Float t = callback->RayCastCallback(r, t_min, t_max, node->primitive);
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
            NodeIndex child1 = node->child1;
            NodeIndex child2 = node->child2;

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