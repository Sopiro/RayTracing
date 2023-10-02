#pragma once

#include "aggregate.h"
#include "area_light.h"
#include "constant_color.h"
#include "directional_light.h"
#include "image_texture.h"
#include "infinite_area_light.h"
#include "intersectable.h"
#include "model.h"
#include "point_light.h"

namespace spt
{

class Scene : public Intersectable
{
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual bool Intersect(Intersection* out_is, const Ray& ray, Float t_min, Float t_max) const override;
    virtual bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const override;
    virtual void GetAABB(AABB* out_aabb) const override;

    void Add(const Ref<Intersectable> object);
    void Add(const Ref<Mesh> object);
    void Add(const Ref<Model> object);

    void AddLight(const Ref<Light> light);
    void AddLight(const Ref<Primitive> object);
    void AddLight(const Ref<Mesh> object);

    const std::vector<Ref<Light>>& GetLights() const;
    const std::vector<InfiniteAreaLight*>& GetInfiniteAreaLights() const;

    void Reset();
    void Rebuild();

private:
    BVH bvh; // Acceleration structure
    std::vector<Ref<Intersectable>> objects;

    std::vector<Ref<Light>> lights;
    std::vector<InfiniteAreaLight*> infinite_lights;
};

inline void Scene::GetAABB(AABB* out_aabb) const
{
    if (bvh.nodeCount == 0)
    {
        out_aabb->min.SetZero();
        out_aabb->max.SetZero();
    }

    *out_aabb = bvh.nodes[bvh.root].aabb;
}

inline void Scene::Add(const Ref<Intersectable> object)
{
    objects.push_back(object);

    AABB aabb;
    object->GetAABB(&aabb);
    bvh.CreateNode(object.get(), aabb);
}

inline void Scene::Add(const Ref<Mesh> mesh)
{
    for (int32 i = 0; i < mesh->triangle_count; ++i)
    {
        auto tri = CreateSharedRef<Triangle>(mesh, i);
        objects.push_back(tri);

        AABB aabb;
        tri->GetAABB(&aabb);
        bvh.CreateNode(tri.get(), aabb);
    }
}

inline void Scene::Add(const Ref<Model> model)
{
    auto& meshes = model->GetMeshes();
    for (size_t i = 0; i < meshes.size(); ++i)
    {
        Add(meshes[i]);
    }
}

inline void Scene::AddLight(const Ref<Light> light)
{
    lights.push_back(light);

    if (light->type == Light::Type::infinite_area_light)
    {
        infinite_lights.push_back((InfiniteAreaLight*)light.get());
    }
}

inline void Scene::AddLight(const Ref<Primitive> primitive)
{
    Add(primitive);
    lights.push_back(CreateSharedRef<AreaLight>(primitive));
}

inline void Scene::AddLight(const Ref<Mesh> mesh)
{
    for (int32 i = 0; i < mesh->triangle_count; ++i)
    {
        auto tri = CreateSharedRef<Triangle>(mesh, i);
        Add(tri);

        lights.push_back(CreateSharedRef<AreaLight>(tri));
    }
}

inline const std::vector<Ref<Light>>& Scene::GetLights() const
{
    return lights;
}

inline const std::vector<InfiniteAreaLight*>& Scene::GetInfiniteAreaLights() const
{
    return infinite_lights;
}

inline void Scene::Reset()
{
    bvh.Reset();
    objects.clear();
    lights.clear();
    infinite_lights.clear();
}

inline void Scene::Rebuild()
{
    bvh.Rebuild();
}

} // namespace spt