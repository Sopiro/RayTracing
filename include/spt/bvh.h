#pragma once

#include "aabb.h"
#include "common.h"
#include "growable_array.h"
#include "intersectable.h"

#define nullNode (-1)

namespace spt
{

constexpr Vec3 aabb_margin{ Real(0.0) };
constexpr Real aabb_multiplier{ Real(1.0) };

inline Real SAH(const AABB& aabb)
{
#if 0
    return aabb.GetArea();
#else
    return aabb.GetPerimater();
#endif
}

class Intersectable;
typedef i32 NodeProxy;
typedef Intersectable Data;

struct Node
{
    bool IsLeaf() const
    {
        return child1 == nullNode;
    }

    i32 id;
    AABB aabb;

    NodeProxy parent;
    NodeProxy child1;
    NodeProxy child2;

    NodeProxy next;
    bool moved;

    Data* data; // user data
};

class BVH : public Intersectable
{
public:
    BVH();
    virtual ~BVH() noexcept;

    BVH(const BVH&) = delete;
    BVH& operator=(const BVH&) = delete;

    BVH(BVH&&) noexcept;
    BVH& operator=(BVH&&) noexcept;

    void Reset();

    NodeProxy CreateNode(Data* data, const AABB& aabb);
    bool MoveNode(NodeProxy node, AABB aabb, const Vec3& displacement, bool force_move);
    void RemoveNode(NodeProxy node);

    bool TestOverlap(NodeProxy nodeA, NodeProxy nodeB) const;
    const AABB& GetAABB(NodeProxy node) const;
    void ClearMoved(NodeProxy node) const;
    bool WasMoved(NodeProxy node) const;
    Data* GetData(NodeProxy node) const;

    void Query(const Vec3& point, const std::function<bool(NodeProxy, Data*)>& callback) const;
    void Query(const AABB& aabb, const std::function<bool(NodeProxy, Data*)>& callback) const;
    template <typename T>
    void Query(const Vec3& point, T* callback) const;
    template <typename T>
    void Query(const AABB& aabb, T* callback) const;

    void RayCast(const Ray& r,
                 Real t_min,
                 Real t_max,
                 const std::function<Real(const Ray&, Real, Real, Intersectable*)>& callback) const;
    template <typename T>
    void RayCast(const Ray& r, Real t_min, Real t_max, T* callback) const;

    void Traverse(const std::function<void(const Node*)>& callback) const;
    template <typename T>
    void Traverse(T* callback) const;

    Real GetTreeCost() const;

    virtual bool Intersect(Intersection* out_is, const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual bool GetAABB(AABB* out_aabb) const override;
    virtual f64 EvaluatePDF(const Ray& ray) const override;
    virtual Vec3 GetRandomDirection(const Vec3& origin) const override;
    virtual i32 GetSize() const override;
    virtual void Rebuild() override;

private:
    i32 size;

    NodeProxy nodeID;
    NodeProxy root;

    Node* nodes;
    i32 nodeCount;
    i32 nodeCapacity;

    NodeProxy freeList;

    std::vector<NodeProxy> leaves;

    NodeProxy AllocateNode();
    void FreeNode(NodeProxy node);

    NodeProxy InsertLeaf(NodeProxy leaf);
    void RemoveLeaf(NodeProxy leaf);

    void Rotate(NodeProxy node);
    void Swap(NodeProxy node1, NodeProxy node2);
};

inline bool BVH::TestOverlap(NodeProxy nodeA, NodeProxy nodeB) const
{
    assert(0 <= nodeA && nodeA < nodeCapacity);
    assert(0 <= nodeB && nodeB < nodeCapacity);

    return nodes[nodeA].aabb.TestOverlap(nodes[nodeB].aabb);
}

inline const AABB& BVH::GetAABB(NodeProxy node) const
{
    assert(0 <= node && node < nodeCapacity);

    return nodes[node].aabb;
}

inline void BVH::ClearMoved(NodeProxy node) const
{
    assert(0 <= node && node < nodeCapacity);

    nodes[node].moved = false;
}

inline bool BVH::WasMoved(NodeProxy node) const
{
    assert(0 <= node && node < nodeCapacity);

    return nodes[node].moved;
}

inline Data* BVH::GetData(NodeProxy node) const
{
    assert(0 <= node && node < nodeCapacity);

    return nodes[node].data;
}

inline Real BVH::GetTreeCost() const
{
    Real cost = Real(0.0);

    Traverse([&cost](const Node* node) -> void { cost += SAH(node->aabb); });

    return cost;
}

template <typename T>
void BVH::Query(const Vec3& point, T* callback) const
{
    if (root == nullNode)
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
void BVH::Query(const AABB& aabb, T* callback) const
{
    if (root == nullNode)
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
void BVH::Traverse(T* callback) const
{
    if (root == nullNode)
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
void BVH::RayCast(const Ray& r, Real t_min, Real t_max, T* callback) const
{
    Vec3 p1 = r.At(t_min);
    Vec3 p2 = r.At(t_max);
    Real t = t_max;

    AABB rayAABB;
    rayAABB.min = Min(p1, p2);
    rayAABB.max = Max(p1, p2);

    GrowableArray<i32, 256> stack;
    stack.Emplace(root);

    while (stack.Count() > 0)
    {
        i32 nodeID = stack.Pop();
        if (nodeID == nullNode)
        {
            continue;
        }

        const Node* node = nodes + nodeID;
        if (node->aabb.TestOverlap(rayAABB) == false)
        {
            continue;
        }

        if (node->aabb.Intersect(r, t_min, t) == false)
        {
            continue;
        }

        if (node->IsLeaf())
        {
            Real value = callback->RayCastCallback(r, t_min, t, node->data);
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

inline bool BVH::GetAABB(AABB* out_aabb) const
{
    if (nodeCount == 0)
    {
        return false;
    }

    *out_aabb = nodes[root].aabb;
    return true;
}

inline f64 BVH::EvaluatePDF(const Ray& ray) const
{
    struct Callback
    {
        f64 sum;
        f64 weight;

        Intersection is;

        f64 RayCastCallback(const Ray& ray, f64 t_min, f64 t_max, Intersectable* object)
        {
            bool hit = object->Intersect(&is, ray, t_min, t_max);

            if (hit)
            {
                sum += weight * is.object->PDFValue(is, ray) / object->GetSize();
            }

            return t_max;
        }
    } callback;

    callback.sum = 0.0;
    callback.weight = 1.0 / leaves.size();

    RayCast(ray, ray_offset, infinity, &callback);

    return callback.sum;
}

inline Vec3 BVH::GetRandomDirection(const Vec3& origin) const
{
    i32 index = static_cast<i32>(leaves.size() * Rand());

    return nodes[leaves[index]].data->GetRandomDirection(origin);
}

inline i32 BVH::GetSize() const
{
    return size;
}

} // namespace spt