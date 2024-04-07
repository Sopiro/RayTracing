#include "bulbit/scene.h"
#include "bulbit/material.h"
#include "bulbit/triangle.h"

namespace bulbit
{

void Scene::AddPrimitive(const Ref<Primitive> primitive)
{
    primitives.push_back(primitive);

    const Material* mat = GetMaterial(primitive->GetMaterialIndex());
    if (mat->IsLightSource())
    {
        // Create area light
        auto area_light = std::make_shared<AreaLight>(primitive);
        area_light->material = mat;
        lights.push_back(area_light);
    }
}

void Scene::AddMesh(const Ref<Mesh> mesh)
{
    for (int32 i = 0; i < mesh->triangle_count; ++i)
    {
        auto tri = std::make_shared<Triangle>(mesh, i);
        AddPrimitive(tri);
    }
}

void Scene::AddModel(Model& model)
{
    MaterialIndex offset = (MaterialIndex)materials.size();
    materials.insert(materials.end(), model.materials.begin(), model.materials.end());

    for (Ref<Mesh> mesh : model.meshes)
    {
        mesh->material += offset;
        AddMesh(mesh);
    }
}

void Scene::AddLight(const Ref<Light> light)
{
    assert(light->type != Light::Type::area_light);

    if (light->type != Light::Type::area_light)
    {
        lights.push_back(light);
    }

    if (light->type == Light::Type::infinite_area_light)
    {
        infinite_lights.push_back((InfiniteAreaLight*)light.get());
    }
}

} // namespace bulbit
