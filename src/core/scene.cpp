#include "bulbit/scene.h"
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

void Scene::Add(const Ref<Primitive> primitive)
{
    primitives.push_back(primitive);

    AABB aabb;
    primitive->GetAABB(&aabb);
    bvh.CreateNode(primitive.get(), aabb);
}

void Scene::Add(const Ref<Mesh> mesh)
{
    for (int32 i = 0; i < mesh->triangle_count; ++i)
    {
        auto tri = CreateSharedRef<Triangle>(mesh, i);
        Add(tri);
    }
}

void Scene::Add(const Ref<Model> model)
{
    for (Ref<Mesh> mesh : model->GetMeshes())
    {
        Add(mesh);
    }
}

void Scene::AddLight(const Ref<Light> light)
{
    lights.push_back(light);

    if (light->type == Light::Type::infinite_area_light)
    {
        infinite_lights.push_back((InfiniteAreaLight*)light.get());
    }
}

void Scene::AddLight(const Ref<Primitive> primitive)
{
    Add(primitive);
    lights.push_back(CreateSharedRef<AreaLight>(primitive));
}

void Scene::AddLight(const Ref<Mesh> mesh)
{
    for (int32 i = 0; i < mesh->triangle_count; ++i)
    {
        auto tri = CreateSharedRef<Triangle>(mesh, i);
        Add(tri);

        lights.push_back(CreateSharedRef<AreaLight>(tri));
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
