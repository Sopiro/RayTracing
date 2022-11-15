#pragma once

#include "aabb.h"
#include "common.h"
#include "hittable.h"

struct Node
{
};

class BVH
{
public:
    BVH() = default;

    int32 Add(const Hittable& object, const AABB& aabb);
    void Build();
};
