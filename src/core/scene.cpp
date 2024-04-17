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
    // Free all pooled scene objects

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

void Scene::AddModel(const Model& model)
{
    for (Ref<Mesh> mesh : model.meshes)
    {
        for (int32 i = 0; i < mesh->triangle_count; ++i)
        {
            CreatePrimitive<Triangle>(mesh, i);
        }
    }
}

void Scene::BuildAccelerationStructure()
{
    // accel.reset(new DynamicBVH(primitives));
    accel.reset(new BVH(primitives));
}

} // namespace bulbit
