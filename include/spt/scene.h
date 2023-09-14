#pragma once

#include "aggregate.h"
#include "common.h"
#include "directional_light.h"
#include "image_texture.h"
#include "intersectable.h"
#include "mesh.h"
#include "model.h"
#include "solid_color.h"
#include "sphere.h"

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

    void Add(const Ref<Intersectable>& object);
    void Add(const Ref<Mesh>& object);
    void Add(const Ref<Model>& object);

    const Ref<Texture>& GetEnvironmentMap() const;
    void SetEnvironmentMap(const Ref<Texture> color);
    Color GetSkyColor(Vec3 direction) const;

    bool HasDirectionalLight() const;
    const Ref<DirectionalLight>& GetDirectionalLight() const;
    void SetDirectionalLight(const Ref<DirectionalLight>& directional_light);

    bool HasAreaLights() const;
    const std::vector<Ref<Primitive>>& GetAreaLights() const;
    void AddAreaLight(const Ref<Primitive>& object);
    void AddAreaLight(const Ref<Mesh>& object);

    void Reset();
    void Rebuild();

private:
    Aggregate accel; // Acceleration structure

    Ref<Texture> environment_map;
    Ref<DirectionalLight> directional_light;
    std::vector<Ref<Primitive>> area_lights;
};

inline Scene::Scene()
    : environment_map{ SolidColor::Create(Color{ 0.0, 0.0, 0.0 }) }
    , directional_light{ nullptr }
{
}

inline bool Scene::Intersect(Intersection* is, const Ray& ray, f64 t_min, f64 t_max) const
{
    return accel.Intersect(is, ray, t_min, t_max);
}

inline bool Scene::IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const
{
    return accel.IntersectAny(ray, t_min, t_max);
}

inline void Scene::GetAABB(AABB* out_aabb) const
{
    return accel.GetAABB(out_aabb);
}

inline void Scene::Add(const Ref<Intersectable>& object)
{
    accel.Add(object);
}

inline void Scene::Add(const Ref<Mesh>& mesh)
{
    accel.Add(mesh);
}

inline void Scene::Add(const Ref<Model>& model)
{
    accel.Add(model);
}

inline const Ref<Texture>& Scene::GetEnvironmentMap() const
{
    return environment_map;
}

inline void Scene::SetEnvironmentMap(const Ref<Texture> env_map)
{
    environment_map = env_map;
}

inline Color Scene::GetSkyColor(Vec3 dir) const
{
    dir.Normalize();

    f64 phi = atan2(-dir.z, dir.x) + pi;
    f64 theta = acos(-dir.y);

    f64 u = phi * inv_two_pi;
    f64 v = theta * inv_pi;

    return environment_map->Value(UV{ u, v }, zero_vec3);
}

inline bool Scene::HasDirectionalLight() const
{
    return directional_light != nullptr;
}

inline const Ref<DirectionalLight>& Scene::GetDirectionalLight() const
{
    return directional_light;
}

inline void Scene::SetDirectionalLight(const Ref<DirectionalLight>& dr)
{
    directional_light = dr;
}

inline bool Scene::HasAreaLights() const
{
    return area_lights.size() > 0;
}

inline const std::vector<Ref<Primitive>>& Scene::GetAreaLights() const
{
    return area_lights;
}

inline void Scene::AddAreaLight(const Ref<Primitive>& object)
{
    area_lights.push_back(object);
}

inline void Scene::AddAreaLight(const Ref<Mesh>& mesh)
{
    auto& vertices = mesh->vertices;
    auto& indices = mesh->indices;

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        u32 index0 = indices[i];
        u32 index1 = indices[i + 1];
        u32 index2 = indices[i + 2];

        auto tri = CreateSharedRef<Triangle>(vertices[index0], vertices[index1], vertices[index2], mesh->material);
        area_lights.push_back(tri);
    }
}

inline void Scene::Reset()
{
    accel.Reset();
    area_lights.clear();
}

inline void Scene::Rebuild()
{
    accel.Rebuild();
}

} // namespace spt