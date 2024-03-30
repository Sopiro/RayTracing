#include "bulbit/dynamic_bvh.h"
#include "bulbit/intersectable.h"
#include "bulbit/util.h"

namespace bulbit
{

DynamicBVH::DynamicBVH()
    : root{ null_node }
    , nodeCapacity{ 32 }
    , nodeCount{ 0 }
{
    nodes = (Node*)malloc(nodeCapacity * sizeof(Node));
    memset(nodes, 0, nodeCapacity * sizeof(Node));

    // Build a linked list for the free list.
    for (int32 i = 0; i < nodeCapacity - 1; ++i)
    {
        nodes[i].next = i + 1;
        nodes[i].parent = i;
    }
    nodes[nodeCapacity - 1].next = null_node;
    nodes[nodeCapacity - 1].parent = nodeCapacity - 1;

    freeList = 0;
}

DynamicBVH::~DynamicBVH() noexcept
{
    free(nodes);
    root = null_node;
    nodeCount = 0;
}

DynamicBVH::DynamicBVH(DynamicBVH&& other) noexcept
{
    // Steal resources
    {
        root = other.root;

        nodes = other.nodes;
        nodeCount = other.nodeCount;
        nodeCapacity = other.nodeCapacity;

        freeList = other.freeList;
    }

    // Clear moved object
    {
        other.root = null_node;

        other.nodes = nullptr;
        other.nodeCount = 0;
        other.nodeCapacity = 0;

        other.freeList = null_node;
    }
}

DynamicBVH& DynamicBVH::operator=(DynamicBVH&& other) noexcept
{
    assert(this != &other);

    free(nodes);

    // Steal resources
    {
        root = other.root;

        nodes = other.nodes;
        nodeCount = other.nodeCount;
        nodeCapacity = other.nodeCapacity;

        freeList = other.freeList;
    }

    // Clear moved object
    {
        other.root = null_node;

        other.nodes = nullptr;
        other.nodeCount = 0;
        other.nodeCapacity = 0;

        other.freeList = null_node;
    }

    return *this;
}

NodeProxy DynamicBVH::InsertLeaf(NodeProxy leaf)
{
    assert(0 <= leaf && leaf < nodeCapacity);
    assert(nodes[leaf].IsLeaf());

    if (root == null_node)
    {
        root = leaf;
        return leaf;
    }

    AABB aabb = nodes[leaf].aabb;

    // Find the best sibling for the new leaf

#if 1
    NodeProxy bestSibling = root;
    Float bestCost = SAH(AABB::Union(nodes[root].aabb, aabb));

    GrowableArray<std::pair<NodeProxy, Float>, 256> stack;
    stack.Emplace(root, 0.0f);

    while (stack.Count() != 0)
    {
        NodeProxy current = stack.Back().first;
        Float inheritedCost = stack.Back().second;
        stack.Pop();

        AABB combined = AABB::Union(nodes[current].aabb, aabb);
        Float directCost = SAH(combined);

        Float cost = directCost + inheritedCost;
        if (cost < bestCost)
        {
            bestCost = cost;
            bestSibling = current;
        }

        inheritedCost += directCost - SAH(nodes[current].aabb);

        Float lowerBoundCost = SAH(aabb) + inheritedCost;
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

        Float area = SAH(nodes[bestSibling].aabb);
        AABB combined = AABB::Union(nodes[bestSibling].aabb, aabb);
        Float combinedArea = SAH(combined);

        Float cost = combinedArea;
        Float inheritanceCost = combinedArea - area;

        Float cost1;
        if (nodes[child1].IsLeaf())
        {
            cost1 = SAH(AABB::Union(nodes[child1].aabb, aabb)) + inheritanceCost;
        }
        else
        {
            Float newArea = SAH(AABB::Union(nodes[child1].aabb, aabb));
            Float oldArea = SAH(nodes[child1].aabb);
            cost1 = (newArea - oldArea) + inheritanceCost; // Lower bound cost required when descending down to child1
        }

        Float cost2;
        if (nodes[child2].IsLeaf())
        {
            cost2 = SAH(AABB::Union(nodes[child2].aabb, aabb)) + inheritanceCost;
        }
        else
        {
            Float newArea = SAH(AABB::Union(nodes[child2].aabb, aabb));
            Float oldArea = SAH(nodes[child2].aabb);
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
    nodes[newParent].aabb = AABB::Union(aabb, nodes[bestSibling].aabb);
    nodes[newParent].data = nullptr;
    nodes[newParent].parent = oldParent;

    // Connect new leaf and sibling to new parent
    nodes[newParent].child1 = bestSibling;
    nodes[newParent].child2 = leaf;
    nodes[bestSibling].parent = newParent;
    nodes[leaf].parent = newParent;

    if (oldParent != null_node)
    {
        if (nodes[oldParent].child1 == bestSibling)
        {
            nodes[oldParent].child1 = newParent;
        }
        else
        {
            nodes[oldParent].child2 = newParent;
        }
    }
    else
    {
        root = newParent;
    }

    // Walk back up the tree refitting ancestors' AABB and applying rotations
    NodeProxy ancestor = newParent;
    while (ancestor != null_node)
    {
        NodeProxy child1 = nodes[ancestor].child1;
        NodeProxy child2 = nodes[ancestor].child2;

        nodes[ancestor].aabb = AABB::Union(nodes[child1].aabb, nodes[child2].aabb);

        Rotate(ancestor);

        ancestor = nodes[ancestor].parent;
    }

    return leaf;
}

void DynamicBVH::RemoveLeaf(NodeProxy leaf)
{
    assert(0 <= leaf && leaf < nodeCapacity);
    assert(nodes[leaf].IsLeaf());

    NodeProxy parent = nodes[leaf].parent;

    if (parent == null_node) // node is root
    {
        assert(root == leaf);
        root = null_node;
        return;
    }

    NodeProxy grandParent = nodes[parent].parent;
    NodeProxy sibling;
    if (nodes[parent].child1 == leaf)
    {
        sibling = nodes[parent].child2;
    }
    else
    {
        sibling = nodes[parent].child1;
    }

    FreeNode(parent);

    if (grandParent != null_node) // node has grandparent
    {
        nodes[sibling].parent = grandParent;

        if (nodes[grandParent].child1 == parent)
        {
            nodes[grandParent].child1 = sibling;
        }
        else
        {
            nodes[grandParent].child2 = sibling;
        }

        NodeProxy ancestor = grandParent;
        while (ancestor != null_node)
        {
            NodeProxy child1 = nodes[ancestor].child1;
            NodeProxy child2 = nodes[ancestor].child2;

            nodes[ancestor].aabb = AABB::Union(nodes[child1].aabb, nodes[child2].aabb);

            Rotate(ancestor);

            ancestor = nodes[ancestor].parent;
        }
    }
    else // node has no grandparent
    {
        root = sibling;
        nodes[sibling].parent = null_node;
    }
}

NodeProxy DynamicBVH::PoolNode(Data* data, const AABB& aabb)
{
    NodeProxy newNode = AllocateNode();

    // Fatten the aabb
    nodes[newNode].aabb.max = aabb.max + aabb_margin;
    nodes[newNode].aabb.min = aabb.min - aabb_margin;
    nodes[newNode].data = data;
    nodes[newNode].parent = null_node;
    nodes[newNode].moved = true;

    data->node = newNode;

    return newNode;
}

NodeProxy DynamicBVH::CreateNode(Data* data, const AABB& aabb)
{
    NodeProxy newNode = AllocateNode();

    // Fatten the aabb
    nodes[newNode].aabb.max = aabb.max + aabb_margin;
    nodes[newNode].aabb.min = aabb.min - aabb_margin;
    nodes[newNode].data = data;
    nodes[newNode].parent = null_node;
    nodes[newNode].moved = true;

    InsertLeaf(newNode);

    data->node = newNode;

    return newNode;
}

bool DynamicBVH::MoveNode(NodeProxy node, AABB aabb, const Vec3& displacement, bool force_move)
{
    assert(0 <= node && node < nodeCapacity);
    assert(nodes[node].IsLeaf());

    const AABB& treeAABB = nodes[node].aabb;
    if (treeAABB.Contains(aabb) && force_move == false)
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

void DynamicBVH::RemoveNode(NodeProxy node)
{
    assert(0 <= node && node < nodeCapacity);
    assert(nodes[node].IsLeaf());

    RemoveLeaf(node);
    FreeNode(node);
}

void DynamicBVH::Rotate(NodeProxy node)
{
    if (nodes[node].IsLeaf())
    {
        return;
    }

    NodeProxy child1 = nodes[node].child1;
    NodeProxy child2 = nodes[node].child2;

    Float costDiffs[4] = { 0.0f };

    if (nodes[child1].IsLeaf() == false)
    {
        Float area1 = SAH(nodes[child1].aabb);
        costDiffs[0] = SAH(AABB::Union(nodes[nodes[child1].child1].aabb, nodes[child2].aabb)) - area1;
        costDiffs[1] = SAH(AABB::Union(nodes[nodes[child1].child2].aabb, nodes[child2].aabb)) - area1;
    }

    if (nodes[child2].IsLeaf() == false)
    {
        Float area2 = SAH(nodes[child2].aabb);
        costDiffs[2] = SAH(AABB::Union(nodes[nodes[child2].child1].aabb, nodes[child1].aabb)) - area2;
        costDiffs[3] = SAH(AABB::Union(nodes[nodes[child2].child2].aabb, nodes[child1].aabb)) - area2;
    }

    int32 bestDiffIndex = 0;
    for (int32 i = 1; i < 4; ++i)
    {
        if (costDiffs[i] < costDiffs[bestDiffIndex])
        {
            bestDiffIndex = i;
        }
    }

    // Rotate only if it reduce the suface area
    if (costDiffs[bestDiffIndex] >= 0.0f)
    {
        return;
    }

    // printf("Tree rotation occurred: %d\n", bestDiffIndex);
    switch (bestDiffIndex)
    {
    case 0:
    {
        // Swap(child2, nodes[child1].child2);
        nodes[nodes[child1].child2].parent = node;
        nodes[node].child2 = nodes[child1].child2;

        nodes[child1].child2 = child2;
        nodes[child2].parent = child1;

        nodes[child1].aabb = AABB::Union(nodes[nodes[child1].child1].aabb, nodes[nodes[child1].child2].aabb);
    }
    break;
    case 1:
    {
        // Swap(child2, nodes[child1].child1);
        nodes[nodes[child1].child1].parent = node;
        nodes[node].child2 = nodes[child1].child1;

        nodes[child1].child1 = child2;
        nodes[child2].parent = child1;

        nodes[child1].aabb = AABB::Union(nodes[nodes[child1].child1].aabb, nodes[nodes[child1].child2].aabb);
    }
    break;
    case 2:
    {
        // Swap(child1, nodes[child2].child2);
        nodes[nodes[child2].child2].parent = node;
        nodes[node].child1 = nodes[child2].child2;

        nodes[child2].child2 = child1;
        nodes[child1].parent = child2;

        nodes[child2].aabb = AABB::Union(nodes[nodes[child2].child1].aabb, nodes[nodes[child2].child2].aabb);
    }
    break;
    case 3:
    {
        // Swap(child1, nodes[child2].child1);
        nodes[nodes[child2].child1].parent = node;
        nodes[node].child1 = nodes[child2].child1;

        nodes[child2].child1 = child1;
        nodes[child1].parent = child2;

        nodes[child2].aabb = AABB::Union(nodes[nodes[child2].child1].aabb, nodes[nodes[child2].child2].aabb);
    }
    break;
    }
}

void DynamicBVH::Swap(NodeProxy node1, NodeProxy node2)
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
    {
        nodes[parent1].child1 = node2;
    }
    else
    {
        nodes[parent1].child2 = node2;
    }
    nodes[node2].parent = parent1;

    if (nodes[parent2].child1 == node2)
    {
        nodes[parent2].child1 = node1;
    }
    else
    {
        nodes[parent2].child2 = node1;
    }
    nodes[node1].parent = parent2;
}

void DynamicBVH::Reset()
{
    root = null_node;
    nodeCount = 0;
    memset(nodes, 0, nodeCapacity * sizeof(Node));

    // Build a linked list for the free list.
    for (int32 i = 0; i < nodeCapacity - 1; ++i)
    {
        nodes[i].next = i + 1;
        nodes[i].parent = i;
    }
    nodes[nodeCapacity - 1].next = null_node;
    nodes[nodeCapacity - 1].parent = nodeCapacity - 1;

    freeList = 0;
}

NodeProxy DynamicBVH::AllocateNode()
{
    if (freeList == null_node)
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
        for (int32 i = nodeCount; i < nodeCapacity - 1; ++i)
        {
            nodes[i].next = i + 1;
            nodes[i].parent = i;
        }
        nodes[nodeCapacity - 1].next = null_node;
        nodes[nodeCapacity - 1].parent = nodeCapacity - 1;

        freeList = nodeCount;
    }

    NodeProxy node = freeList;
    freeList = nodes[node].next;
    nodes[node].parent = null_node;
    nodes[node].child1 = null_node;
    nodes[node].child2 = null_node;
    nodes[node].moved = false;
    ++nodeCount;

    return node;
}

void DynamicBVH::FreeNode(NodeProxy node)
{
    assert(0 <= node && node <= nodeCapacity);
    assert(0 < nodeCount);

    nodes[node].parent = node;
    nodes[node].next = freeList;
    freeList = node;
    --nodeCount;
}

void DynamicBVH::Rebuild()
{
    NodeProxy* primitives = (NodeProxy*)malloc(nodeCount * sizeof(NodeProxy));
    int32 count = 0;

    // Collect all primitives
    for (int32 i = 0; i < nodeCapacity; ++i)
    {
        // Already in the free list
        if (nodes[i].parent == i)
        {
            continue;
        }

        // Clean the leaf
        if (nodes[i].IsLeaf())
        {
            nodes[i].parent = null_node;
            primitives[count++] = i;
        }
        else
        {
            // Free the internal node
            FreeNode(i);
        }
    }

    struct BuildNode
    {
        NodeProxy node;
        int32 begin, end;
    };

    root = AllocateNode();

    std::vector<BuildNode> stack;
    stack.reserve(2 * count - 1);

    stack.emplace_back(root, 0, count);

    // Subdivision
    while (stack.size() > 0)
    {
        BuildNode n = stack.back();
        stack.pop_back();

        NodeProxy node = n.node;

        AABB& aabb = nodes[node].aabb;
        for (int32 i = n.begin; i < n.end; ++i)
        {
            aabb = AABB::Union(aabb, nodes[primitives[i]].aabb);
        }
        nodes[node].aabb = aabb;

        if (aabb.GetSurfaceArea() == 0.0f)
        {
            std::cout << "something went wrong" << std::endl;
        }

        // get longest axis
        Vec3 extents = aabb.GetExtents();
        int32 axis = 0;
        if (extents.y > extents.x)
        {
            axis = 1;
        }
        if (extents.z > extents[axis])
        {
            axis = 2;
        }
        Float split_pos = aabb.min[axis] + extents[axis] * 0.5f;

        NodeProxy* m = std::partition(primitives + n.begin, primitives + n.end,
                                      [=](NodeProxy n) { return nodes[n].aabb.GetCenter()[axis] < split_pos; });
        int32 mid = int32(m - primitives);

        int32 left_count = mid - n.begin;
        int32 right_count = n.end - mid;

        if (left_count == 0 || right_count == 0)
        {
            mid = (n.end + n.begin) / 2;

            std::nth_element(primitives + n.begin, primitives + mid, primitives + n.end, [=](NodeProxy a, NodeProxy b) {
                return nodes[a].aabb.GetCenter()[axis] < nodes[b].aabb.GetCenter()[axis];
            });
        }

        left_count = mid - n.begin;
        right_count = n.end - mid;

        if (left_count == 1)
        {
            nodes[node].child1 = primitives[n.begin];
            nodes[primitives[n.begin]].parent = node;
        }
        else
        {
            NodeProxy left = AllocateNode();
            nodes[node].child1 = left;
            nodes[left].parent = node;
            stack.emplace_back(left, n.begin, mid);
        }

        if (right_count == 1)
        {
            nodes[node].child2 = primitives[mid];
            nodes[primitives[mid]].parent = node;
        }
        else
        {
            NodeProxy right = AllocateNode();
            nodes[node].child2 = right;
            nodes[right].parent = node;
            stack.emplace_back(right, mid, n.end);
        }
    }

    free(primitives);
}

} // namespace bulbit