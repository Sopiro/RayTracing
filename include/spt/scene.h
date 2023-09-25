#pragma once

#include "aggregate.h"
#include "area_light.h"
#include "directional_light.h"
#include "image_texture.h"
#include "intersectable.h"
#include "model.h"
#include "point_light.h"
#include "solid_color.h"

namespace spt
{

class Scene : public Intersectable
{
public:
    Scene();
    virtual ~Scene() = default;

    virtual bool Intersect(Intersection* out_is, const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual bool IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual void GetAABB(AABB* out_aabb) const override;

    void Add(const Ref<Intersectable> object);
    void Add(const Ref<Mesh> object);
    void Add(const Ref<Model> object);

    void AddLight(const Ref<Primitive> object);
    void AddLight(const Ref<Mesh> object);
    void AddLight(const Ref<DirectionalLight> directional_light);
    void AddLight(const Ref<PointLight> point_light);

    bool HasLights() const;
    const std::vector<Ref<Light>>& GetLights() const;

    const Ref<Texture> GetEnvironmentMap() const;
    void SetEnvironmentMap(const Ref<Texture> color);
    Color GetSkyColor(const Vec3& direction) const;

    void Reset();
    void Rebuild();

private:
    // Acceleration structure
    BVH bvh;
    std::vector<Ref<Intersectable>> objects;
    std::vector<Ref<Light>> lights;

    Ref<Texture> environment_map; // todo: importance sample this
};

inline Scene::Scene()
    : environment_map{ SolidColor::Create(Color(0.0, 0.0, 0.0)) }
{
}

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
    for (i32 i = 0; i < mesh->triangle_count; ++i)
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

inline void Scene::AddLight(const Ref<Primitive> primitive)
{
    Add(primitive);
    lights.push_back(CreateSharedRef<AreaLight>(primitive));
}

inline void Scene::AddLight(const Ref<Mesh> mesh)
{
    for (i32 i = 0; i < mesh->triangle_count; ++i)
    {
        auto tri = CreateSharedRef<Triangle>(mesh, i);
        Add(tri);

        lights.push_back(CreateSharedRef<AreaLight>(tri));
    }
}

inline void Scene::AddLight(const Ref<DirectionalLight> directional_light)
{
    lights.push_back(directional_light);
}

inline void Scene::AddLight(const Ref<PointLight> point_light)
{
    lights.push_back(point_light);
}

inline bool Scene::HasLights() const
{
    return lights.size() > 0;
}

inline const std::vector<Ref<Light>>& Scene::GetLights() const
{
    return lights;
}

inline const Ref<Texture> Scene::GetEnvironmentMap() const
{
    return environment_map;
}

inline void Scene::SetEnvironmentMap(const Ref<Texture> env_map)
{
    environment_map = env_map;
}

inline Color Scene::GetSkyColor(const Vec3& dir) const
{
    return environment_map->Value(ComputeSphereUV(dir));
}

inline void Scene::Reset()
{
    bvh.Reset();
    objects.clear();
    lights.clear();
}

inline void Scene::Rebuild()
{
    bvh.Rebuild();
}

} // namespace spt