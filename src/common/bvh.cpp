#include "spt/bvh.h"
#include "spt/hittable.h"

namespace spt
{

BVH::BVH()
    : nodeID{ 0 }
    , root{ nullNode }
    , nodeCapacity{ 32 }
    , nodeCount{ 0 }
{
    nodes = (Node*)malloc(nodeCapacity * sizeof(Node));
    memset(nodes, 0, nodeCapacity * sizeof(Node));

    // Build a linked list for the free list.
    for (i32 i = 0; i < nodeCapacity - 1; ++i)
    {
        nodes[i].next = i + 1;
    }
    nodes[nodeCapacity - 1].next = nullNode;
    freeList = 0;

    size = 0;
}

BVH::~BVH() noexcept
{
    free(nodes);
    root = nullNode;
    nodeCount = 0;
}

BVH::BVH(BVH&& other) noexcept
{
    // Steal resources
    {
        size = other.size;

        nodeID = other.nodeID;
        root = other.root;

        nodes = other.nodes;
        nodeCount = other.nodeCount;
        nodeCapacity = other.nodeCapacity;

        freeList = other.freeList;

        leaves = std::move(other.leaves);
    }

    // Clear moved object
    {
        other.size = 0;

        other.nodeID = 0;
        other.root = nullNode;

        other.nodes = nullptr;
        other.nodeCount = 0;
        other.nodeCapacity = 0;

        other.freeList = nullNode;
    }
}

BVH& BVH::operator=(BVH&& other) noexcept
{
    assert(this != &other);

    free(nodes);

    // Steal resources
    {
        size = other.size;

        nodeID = other.nodeID;
        root = other.root;

        nodes = other.nodes;
        nodeCount = other.nodeCount;
        nodeCapacity = other.nodeCapacity;

        freeList = other.freeList;

        leaves = std::move(other.leaves);
    }

    // Clear moved object
    {
        other.size = 0;

        other.nodeID = 0;
        other.root = nullNode;

        other.nodes = nullptr;
        other.nodeCount = 0;
        other.nodeCapacity = 0;

        other.freeList = nullNode;
    }

    return *this;
}

NodeProxy BVH::InsertLeaf(NodeProxy leaf)
{
    assert(0 <= leaf && leaf < nodeCapacity);
    assert(nodes[leaf].IsLeaf());

    if (root == nullNode)
    {
        root = leaf;
        return leaf;
    }

    AABB aabb = nodes[leaf].aabb;

    // Find the best sibling for the new leaf

#if 1
    NodeProxy bestSibling = root;
    Real bestCost = SAH(Union(nodes[root].aabb, aabb));

    GrowableArray<std::pair<NodeProxy, Real>, 256> stack;
    stack.Emplace(root, 0.0f);

    while (stack.Count() != 0)
    {
        NodeProxy current = stack.Back().first;
        Real inheritedCost = stack.Back().second;
        stack.Pop();

        AABB combined = Union(nodes[current].aabb, aabb);
        Real directCost = SAH(combined);

        Real cost = directCost + inheritedCost;
        if (cost < bestCost)
        {
            bestCost = cost;
            bestSibling = current;
        }

        inheritedCost += directCost - SAH(nodes[current].aabb);

        Real lowerBoundCost = SAH(aabb) + inheritedCost;
        if (lowerBoundCost < bestCost)
        {
            if (nodes[current].IsLeaf() == false)
            {
                stack.Emplace(nodes[current].child1, inheritedCost);
                stack.Emplace(nodes[current].child2, inheritedCost);
            }
        }
    }
#else
    // O(log n)
    // This method is faster when inserting a new node, but builds a slightly bad quality tree.
    NodeProxy bestSibling = root;
    while (nodes[bestSibling].IsLeaf() == false)
    {
        NodeProxy child1 = nodes[bestSibling].child1;
        NodeProxy child2 = nodes[bestSibling].child2;

        Real area = SAH(nodes[bestSibling].aabb);
        AABB combined = Union(nodes[bestSibling].aabb, aabb);
        Real combinedArea = SAH(combined);

        Real cost = combinedArea;
        Real inheritanceCost = combinedArea - area;

        Real cost1;
        if (nodes[child1].IsLeaf())
        {
            cost1 = SAH(Union(nodes[child1].aabb, aabb)) + inheritanceCost;
        }
        else
        {
            Real newArea = SAH(Union(nodes[child1].aabb, aabb));
            Real oldArea = SAH(nodes[child1].aabb);
            cost1 = (newArea - oldArea) + inheritanceCost; // Lower bound cost required when descending down to child1
        }

        Real cost2;
        if (nodes[child2].IsLeaf())
        {
            cost2 = SAH(Union(nodes[child2].aabb, aabb)) + inheritanceCost;
        }
        else
        {
            Real newArea = SAH(Union(nodes[child2].aabb, aabb));
            Real oldArea = SAH(nodes[child2].aabb);
            cost2 = (newArea - oldArea) + inheritanceCost; // Lower bound cost required when descending down to child2
        }

        if (cost < cost1 && cost < cost2)
        {
            break;
        }

        if (cost1 < cost2)
        {
            bestSibling = child1;
        }
        else
        {
            bestSibling = child2;
        }
    }
#endif

    // Create a new parent
    NodeProxy oldParent = nodes[bestSibling].parent;
    NodeProxy newParent = AllocateNode();
    nodes[newParent].aabb = Union(aabb, nodes[bestSibling].aabb);
    nodes[newParent].data = nullptr;
    nodes[newParent].parent = oldParent;

    if (oldParent != nullNode)
    {
        if (nodes[oldParent].child1 == bestSibling)
        {
            nodes[oldParent].child1 = newParent;
        }
        else
        {
            nodes[oldParent].child2 = newParent;
        }

        nodes[newParent].child1 = bestSibling;
        nodes[newParent].child2 = leaf;
        nodes[bestSibling].parent = newParent;
        nodes[leaf].parent = newParent;
    }
    else
    {
        nodes[newParent].child1 = bestSibling;
        nodes[newParent].child2 = leaf;
        nodes[bestSibling].parent = newParent;
        nodes[leaf].parent = newParent;
        root = newParent;
    }

    // Walk back up the tree refitting ancestors' AABB and applying rotations
    NodeProxy ancestor = nodes[leaf].parent;
    while (ancestor != nullNode)
    {
        NodeProxy child1 = nodes[ancestor].child1;
        NodeProxy child2 = nodes[ancestor].child2;

        nodes[ancestor].aabb = Union(nodes[child1].aabb, nodes[child2].aabb);

        Rotate(ancestor);

        ancestor = nodes[ancestor].parent;
    }

    return leaf;
}

void BVH::RemoveLeaf(NodeProxy leaf)
{
    assert(0 <= leaf && leaf < nodeCapacity);
    assert(nodes[leaf].IsLeaf());

    NodeProxy parent = nodes[leaf].parent;

    if (parent != nullNode) // node is not root
    {
        NodeProxy sibling = nodes[parent].child1 == leaf ? nodes[parent].child2 : nodes[parent].child1;

        if (nodes[parent].parent != nullNode) // sibling has grandparent
        {
            nodes[sibling].parent = nodes[parent].parent;

            NodeProxy grandParent = nodes[parent].parent;
            if (nodes[grandParent].child1 == parent)
            {
                nodes[grandParent].child1 = sibling;
            }
            else
            {
                nodes[grandParent].child2 = sibling;
            }
        }
        else // sibling has no grandparent
        {
            root = sibling;

            nodes[sibling].parent = nullNode;
        }

        FreeNode(parent);

        NodeProxy ancestor = nodes[sibling].parent;
        while (ancestor != nullNode)
        {
            NodeProxy child1 = nodes[ancestor].child1;
            NodeProxy child2 = nodes[ancestor].child2;

            nodes[ancestor].aabb = Union(nodes[child1].aabb, nodes[child2].aabb);

            ancestor = nodes[ancestor].parent;
        }
    }
    else // node is root
    {
        assert(root == leaf);

        root = nullNode;
    }
}

NodeProxy BVH::CreateNode(Data* data, const AABB& aabb)
{
    NodeProxy newNode = AllocateNode();

    // Fatten the aabb
    nodes[newNode].aabb.max = aabb.max + aabb_margin;
    nodes[newNode].aabb.min = aabb.min - aabb_margin;
    nodes[newNode].data = data;
    nodes[newNode].parent = nullNode;
    nodes[newNode].moved = true;

    InsertLeaf(newNode);
    leaves.push_back(newNode);

    size += data->GetSize();

    return newNode;
}

bool BVH::MoveNode(NodeProxy node, AABB aabb, const Vec3& displacement, bool forceMove)
{
    assert(0 <= node && node < nodeCapacity);
    assert(nodes[node].IsLeaf());

    const AABB& treeAABB = nodes[node].aabb;
    if (treeAABB.Contains(aabb) && forceMove == false)
    {
        return false;
    }

    Vec3 d = displacement * aabb_multiplier;

    if (d.x > 0.0f)
    {
        aabb.max.x += d.x;
    }
    else
    {
        aabb.min.x += d.x;
    }

    if (d.y > 0.0f)
    {
        aabb.max.y += d.y;
    }
    else
    {
        aabb.min.y += d.y;
    }

    // Fatten the aabb
    aabb.max += aabb_margin;
    aabb.min -= aabb_margin;

    RemoveLeaf(node);

    nodes[node].aabb = aabb;

    InsertLeaf(node);

    nodes[node].moved = true;

    return true;
}

void BVH::RemoveNode(NodeProxy node)
{
    assert(0 <= node && node < nodeCapacity);
    assert(nodes[node].IsLeaf());

    size -= nodes[node].data->GetSize();

    RemoveLeaf(node);

    for (i32 i = 0; i < leaves.size(); ++i)
    {
        if (leaves[i] == node)
        {
            leaves[i] = leaves.back();
        }
    }

    leaves.pop_back();

    FreeNode(node);
}

void BVH::Rotate(NodeProxy node)
{
    if (nodes[node].IsLeaf())
    {
        return;
    }

    if (nodes[node].parent == nullNode)
    {
        return;
    }

    NodeProxy parent = nodes[node].parent;

    NodeProxy sibling;
    if (nodes[parent].child1 == node)
    {
        sibling = nodes[parent].child2;
    }
    else
    {
        sibling = nodes[parent].child1;
    }

    i32 count = 2;
    Real costDiffs[4];
    Real nodeArea = SAH(nodes[node].aabb);

    costDiffs[0] = SAH(Union(nodes[sibling].aabb, nodes[nodes[node].child1].aabb)) - nodeArea;
    costDiffs[1] = SAH(Union(nodes[sibling].aabb, nodes[nodes[node].child2].aabb)) - nodeArea;

    if (nodes[sibling].IsLeaf() == false)
    {
        Real siblingArea = SAH(nodes[sibling].aabb);
        costDiffs[2] = SAH(Union(nodes[node].aabb, nodes[nodes[sibling].child1].aabb)) - siblingArea;
        costDiffs[3] = SAH(Union(nodes[node].aabb, nodes[nodes[sibling].child2].aabb)) - siblingArea;

        count += 2;
    }

    i32 bestDiffIndex = 0;
    for (i32 i = 1; i < count; ++i)
    {
        if (costDiffs[i] < costDiffs[bestDiffIndex])
        {
            bestDiffIndex = i;
        }
    }

    // Rotate only if it reduce the suface area
    if (costDiffs[bestDiffIndex] < 0.0f)
    {
        // printf("Tree rotation occurred: %d\n", bestDiffIndex);

        switch (bestDiffIndex)
        {
        case 0: // Swap(sibling, node->child2);
            if (nodes[parent].child1 == sibling)
                nodes[parent].child1 = nodes[node].child2;
            else
                nodes[parent].child2 = nodes[node].child2;

            nodes[nodes[node].child2].parent = parent;

            nodes[node].child2 = sibling;
            nodes[sibling].parent = node;

            nodes[node].aabb = Union(nodes[sibling].aabb, nodes[nodes[node].child1].aabb);
            break;
        case 1: // Swap(sibling, node->child1);
            if (nodes[parent].child1 == sibling)
                nodes[parent].child1 = nodes[node].child1;
            else
                nodes[parent].child2 = nodes[node].child1;

            nodes[nodes[node].child1].parent = parent;

            nodes[node].child1 = sibling;
            nodes[sibling].parent = node;

            nodes[node].aabb = Union(nodes[sibling].aabb, nodes[nodes[node].child2].aabb);
            break;
        case 2: // Swap(node, sibling->child2);
            if (nodes[parent].child1 == node)
                nodes[parent].child1 = nodes[sibling].child2;
            else
                nodes[parent].child2 = nodes[sibling].child2;

            nodes[nodes[sibling].child2].parent = parent;

            nodes[sibling].child2 = node;
            nodes[node].parent = sibling;

            nodes[sibling].aabb = Union(nodes[node].aabb, nodes[nodes[sibling].child2].aabb);
            break;
        case 3: // Swap(node, sibling->child1);
            if (nodes[parent].child1 == node)
                nodes[parent].child1 = nodes[sibling].child1;
            else
                nodes[parent].child2 = nodes[sibling].child1;

            nodes[nodes[sibling].child1].parent = parent;

            nodes[sibling].child1 = node;
            nodes[node].parent = sibling;

            nodes[sibling].aabb = Union(nodes[node].aabb, nodes[nodes[sibling].child1].aabb);
            break;
        }
    }
}

void BVH::Swap(NodeProxy node1, NodeProxy node2)
{
    NodeProxy parent1 = nodes[node1].parent;
    NodeProxy parent2 = nodes[node2].parent;

    if (parent1 == parent2)
    {
        nodes[parent1].child1 = node2;
        nodes[parent1].child2 = node1;
        return;
    }

    if (nodes[parent1].child1 == node1)
        nodes[parent1].child1 = node2;
    else
        nodes[parent1].child2 = node2;
    nodes[node2].parent = parent1;

    if (nodes[parent2].child1 == node2)
        nodes[parent2].child1 = node1;
    else
        nodes[parent2].child2 = node1;
    nodes[node1].parent = parent2;
}

void BVH::Query(const Vec3& point, const std::function<bool(NodeProxy, Data*)>& callback) const
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

        if (!nodes[current].aabb.TestPoint(point))
        {
            continue;
        }

        if (nodes[current].IsLeaf())
        {
            bool proceed = callback(current, nodes[current].data);
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

void BVH::Query(const AABB& aabb, const std::function<bool(NodeProxy, Data*)>& callback) const
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

        if (!nodes[current].aabb.TestOverlap(aabb))
        {
            continue;
        }

        if (nodes[current].IsLeaf())
        {
            bool proceed = callback(current, nodes[current].data);
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

void BVH::Traverse(const std::function<void(const Node*)>& callback) const
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
        callback(node);
    }
}

void BVH::Reset()
{
    nodeID = 0;
    root = nullNode;
    nodeCount = 0;
    memset(nodes, 0, nodeCapacity * sizeof(Node));

    // Build a linked list for the free list.
    for (i32 i = 0; i < nodeCapacity - 1; ++i)
    {
        nodes[i].next = i + 1;
    }
    nodes[nodeCapacity - 1].next = nullNode;
    freeList = 0;
}

NodeProxy BVH::AllocateNode()
{
    if (freeList == nullNode)
    {
        assert(nodeCount == nodeCapacity);

        // Grow the node pool
        Node* oldNodes = nodes;
        nodeCapacity *= 2;
        nodes = (Node*)malloc(nodeCapacity * sizeof(Node));
        memcpy(nodes, oldNodes, nodeCount * sizeof(Node));
        memset(nodes + nodeCount, 0, nodeCount * sizeof(Node));
        free(oldNodes);

        // Build a linked list for the free list.
        for (i32 i = nodeCount; i < nodeCapacity - 1; ++i)
        {
            nodes[i].next = i + 1;
        }
        nodes[nodeCapacity - 1].next = nullNode;
        freeList = nodeCount;
    }

    NodeProxy node = freeList;
    freeList = nodes[node].next;
    nodes[node].id = ++nodeID;
    nodes[node].parent = nullNode;
    nodes[node].child1 = nullNode;
    nodes[node].child2 = nullNode;
    nodes[node].moved = false;
    ++nodeCount;

    return node;
}

void BVH::FreeNode(NodeProxy node)
{
    assert(0 <= node && node <= nodeCapacity);
    assert(0 < nodeCount);

    nodes[node].id = 0;
    nodes[node].next = freeList;
    freeList = node;
    --nodeCount;
}

void BVH::Rebuild()
{
    // Rebuild the tree with bottom up approach

    NodeProxy* leaves = (NodeProxy*)malloc(nodeCount * sizeof(NodeProxy));
    i32 count = 0;

    // Build an array of leaf node
    for (i32 i = 0; i < nodeCapacity; ++i)
    {
        // Already in the free list
        if (nodes[i].id == 0)
        {
            continue;
        }

        // Clean the leaf
        if (nodes[i].IsLeaf())
        {
            nodes[i].parent = nullNode;

            leaves[count++] = i;
        }
        else
        {
            // Free the internal node
            FreeNode(i);
        }
    }

    while (count > 1)
    {
        Real minCost = infinity;
        i32 minI = -1;
        i32 minJ = -1;

        // Find the best aabb pair
        for (i32 i = 0; i < count; ++i)
        {
            AABB aabbI = nodes[leaves[i]].aabb;

            for (i32 j = i + 1; j < count; ++j)
            {
                AABB aabbJ = nodes[leaves[j]].aabb;

                AABB combined = Union(aabbI, aabbJ);
                Real cost = SAH(combined);

                if (cost < minCost)
                {
                    minCost = cost;
                    minI = i;
                    minJ = j;
                }
            }
        }

        NodeProxy index1 = leaves[minI];
        NodeProxy index2 = leaves[minJ];
        Node* child1 = nodes + index1;
        Node* child2 = nodes + index2;

        // Create a parent(internal) node
        NodeProxy parentIndex = AllocateNode();
        Node* parent = nodes + parentIndex;

        parent->child1 = index1;
        parent->child2 = index2;
        parent->aabb = Union(child1->aabb, child2->aabb);
        parent->parent = nullNode;

        child1->parent = parentIndex;
        child2->parent = parentIndex;

        leaves[minI] = parentIndex;

        leaves[minJ] = leaves[count - 1];
        --count;
    }

    root = leaves[0];
    free(leaves);
}

void BVH::RayCast(const Ray& r,
                  Real t_min,
                  Real t_max,
                  const std::function<Real(const Ray&, Real, Real, Hittable*)>& callback) const
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

        if (node->aabb.Hit(r, t_min, t) == false)
        {
            continue;
        }

        if (node->IsLeaf())
        {
            Real value = callback(r, t_min, t, node->data);
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

bool BVH::Hit(const Ray& ray, f64 t_min, f64 t_max, HitRecord& rec) const
{
    struct Callback
    {
        HitRecord* rec;
        bool hit_closest;
        f64 t;

        f64 RayCastCallback(const Ray& ray, f64 t_min, f64 t_max, Hittable* object)
        {
            bool hit = object->Hit(ray, t_min, t_max, *rec);

            if (hit)
            {
                hit_closest = true;
                t = rec->t;
            }

            // Keep traverse with smaller bounds
            return t;
        }
    } callback;

    callback.rec = &rec;
    callback.hit_closest = false;
    callback.t = t_max;

    RayCast(ray, t_min, t_max, &callback);

    return callback.hit_closest;
}

} // namespace spt