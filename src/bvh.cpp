#include "raytracer/bvh.h"
#include "raytracer/hittable.h"

BVH::BVH()
{
    nodeID = 0;
    root = nullNode;
    nodeCapacity = 32;
    nodeCount = 0;
    nodes = (Node*)malloc(nodeCapacity * sizeof(Node));
    memset(nodes, 0, nodeCapacity * sizeof(Node));

    // Build a linked list for the free list.
    for (int32 i = 0; i < nodeCapacity - 1; ++i)
    {
        nodes[i].next = i + 1;
    }
    nodes[nodeCapacity - 1].next = nullNode;
    freeList = 0;
}

BVH::~BVH()
{
    free(nodes);
    root = nullNode;
    nodeCount = 0;
}

int32 BVH::AllocateNode()
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
        for (int32 i = nodeCount; i < nodeCapacity - 1; ++i)
        {
            nodes[i].next = i + 1;
        }
        nodes[nodeCapacity - 1].next = nullNode;
        freeList = nodeCount;
    }

    int32 node = freeList;
    freeList = nodes[node].next;
    nodes[node].id = ++nodeID;
    nodes[node].parent = nullNode;
    nodes[node].child1 = nullNode;
    nodes[node].child2 = nullNode;
    ++nodeCount;

    return node;
}

void BVH::FreeNode(int32 node)
{
    assert(0 <= node && node <= nodeCapacity);
    assert(0 < nodeCount);

    nodes[node].id = 0;
    nodes[node].next = freeList;
    freeList = node;
    --nodeCount;
}

int32 BVH::Add(Hittable* object, const AABB& aabb)
{
    int32 newNode = AllocateNode();

    nodes[newNode].aabb = aabb;
    nodes[newNode].isLeaf = true;
    nodes[newNode].body = object;
    nodes[newNode].parent = nullNode;
    object->node = newNode;

    return newNode;
}

void BVH::Build()
{
    int32* leaves = (int32*)malloc(nodeCount * sizeof(Node));
    int32 count = 0;

    // Build an array of leaf node
    for (int32 i = 0; i < nodeCapacity; ++i)
    {
        // Already in the free list
        if (nodes[i].id == 0)
        {
            continue;
        }

        // Clean the leaf
        if (nodes[i].isLeaf)
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
        double minCost = DBL_MAX;
        int32 minI = -1;
        int32 minJ = -1;

        // Find the best aabb pair
        for (int32 i = 0; i < count; ++i)
        {
            AABB aabbI = nodes[leaves[i]].aabb;

            for (int32 j = i + 1; j < count; ++j)
            {
                AABB aabbJ = nodes[leaves[j]].aabb;

                AABB combined = Union(aabbI, aabbJ);
                double cost = SAH(combined);

                if (cost < minCost)
                {
                    minCost = cost;
                    minI = i;
                    minJ = j;
                }
            }
        }

        int32 index1 = leaves[minI];
        int32 index2 = leaves[minJ];
        Node* child1 = nodes + index1;
        Node* child2 = nodes + index2;

        // Create a parent(internal) node
        int32 parentIndex = AllocateNode();
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

double BVH::ComputeCost() const
{
    double cost = 0.0;

    if (root == nullNode)
    {
        return cost;
    }

    GrowableArray<int32, 256> stack;
    stack.Emplace(root);

    while (stack.Count() != 0)
    {
        int32 current = stack.Pop();

        if (!nodes[current].isLeaf)
        {
            stack.Emplace(nodes[current].child1);
            stack.Emplace(nodes[current].child2);
        }

        const Node* node = nodes + current;

        cost += SAH(node->aabb);
    }

    return cost;
}