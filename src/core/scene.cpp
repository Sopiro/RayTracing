#include "bulbit/scene.h"
#include "bulbit/material.h"
#include "bulbit/triangle.h"

namespace bulbit
{

void Scene::GetAABB(AABB* out_aabb) const
{
    if (bvh.nodeCount == 0)
    {
        out_aabb->min.SetZero();
        out_aabb->max.SetZero();
    }

    *out_aabb = bvh.nodes[bvh.root].aabb;
}

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

    AABB aabb;
    primitive->GetAABB(&aabb);
    bvh.CreateNode(primitive.get(), aabb);
}

void Scene::AddMesh(const Ref<Mesh> mesh)
{
    for (int32 i = 0; i < mesh->triangle_count; ++i)
    {
        auto tri = std::make_shared<Triangle>(mesh, i);
        AddPrimitive(tri);
    }
}

void Scene::AddModel(const Ref<Model> model)
{
    MaterialIndex offset = (MaterialIndex)materials.size();
    materials.insert(materials.end(), model->materials.begin(), model->materials.end());

    for (Ref<Mesh> mesh : model->GetMeshes())
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

bool Scene::Intersect(Intersection* is, const Ray& ray, Float t_min, Float t_max) const
{
    struct Callback
    {
        Intersection* is;
        bool hit_closest;
        Float t;

        Float RayCastCallback(const Ray& ray, Float t_min, Float t_max, Intersectable* object)
        {
            bool hit = object->Intersect(is, ray, t_min, t_max);

            if (hit)
            {
                hit_closest = true;
                t = is->t;
            }

            // Keep traverse with smaller bounds
            return t;
        }
    } callback;

    callback.is = is;
    callback.hit_closest = false;
    callback.t = t_max;

    bvh.RayCast(ray, t_min, t_max, &callback);

    return callback.hit_closest;
}

bool Scene::IntersectAny(const Ray& ray, Float t_min, Float t_max) const
{
    struct Callback
    {
        bool hit_any;

        Float RayCastCallback(const Ray& ray, Float t_min, Float t_max, Intersectable* object)
        {
            bool hit = object->IntersectAny(ray, t_min, t_max);

            if (hit)
            {
                hit_any = true;

                // Stop traversal
                return t_min;
            }

            return t_max;
        }
    } callback;

    callback.hit_any = false;

    bvh.RayCast(ray, t_min, t_max, &callback);

    return callback.hit_any;
}

} // namespace bulbit
