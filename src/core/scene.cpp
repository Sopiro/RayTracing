#include "bulbit/scene.h"
#include "bulbit/material.h"
#include "bulbit/sphere.h"
#include "bulbit/triangle.h"

namespace bulbit
{

Scene::Scene()
    : resource{ 64 * 1024 }
    , pool{ &resource }
    , allocator{ &pool }
{
}

Scene::~Scene()
{
    // Free all pooled resources

    for (Primitive* p : primitives)
    {
        allocator.delete_object(p);
    }

    for (Material* m : materials)
    {
        allocator.delete_object(m);
    }

    for (Light* l : lights)
    {
        allocator.delete_object(l);
    }
}

void Scene::BuildAccelerationStructure()
{
    // accel.reset(new DynamicBVH(primitives));
    accel.reset(new BVH(primitives));
}

} // namespace bulbit
