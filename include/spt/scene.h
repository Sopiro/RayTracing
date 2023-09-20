#pragma once

#include "aggregate.h"
#include "area_light.h"
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

    void Add(const Ref<Intersectable> object);
    void Add(const Ref<Mesh> object);
    void Add(const Ref<Model> object);

    const Ref<Texture> GetEnvironmentMap() const;
    void SetEnvironmentMap(const Ref<Texture> color);
    Color GetSkyColor(const Vec3& direction) const;

    bool HasDirectionalLight() const;
    const Ref<DirectionalLight> GetDirectionalLight() const;
    void SetDirectionalLight(const Ref<DirectionalLight> directional_light);

    bool HasLights() const;
    const std::vector<Ref<Light>>& GetLights() const;
    void AddLight(const Ref<Primitive> object);
    void AddLight(const Ref<Mesh> object);

    void Reset();
    void Rebuild();

private:
    Aggregate accel; // Acceleration structure

    Ref<Texture> environment_map;
    Ref<DirectionalLight> directional_light;
    std::vector<Ref<Light>> lights;
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

inline void Scene::Add(const Ref<Intersectable> object)
{
    accel.Add(object);
}

inline void Scene::Add(const Ref<Mesh> mesh)
{
    accel.Add(mesh);
}

inline void Scene::Add(const Ref<Model> model)
{
    accel.Add(model);
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

inline bool Scene::HasDirectionalLight() const
{
    return directional_light != nullptr;
}

inline const Ref<DirectionalLight> Scene::GetDirectionalLight() const
{
    return directional_light;
}

inline void Scene::SetDirectionalLight(const Ref<DirectionalLight> dr)
{
    directional_light = dr;
}

inline bool Scene::HasLights() const
{
    return lights.size() > 0;
}

inline const std::vector<Ref<Light>>& Scene::GetLights() const
{
    return lights;
}

inline void Scene::AddLight(const Ref<Primitive> primitive)
{
    lights.push_back(CreateSharedRef<AreaLight>(primitive));
    accel.Add(primitive);
}

inline void Scene::AddLight(const Ref<Mesh> mesh)
{
    for (i32 i = 0; i < mesh->triangle_count; ++i)
    {
        auto tri = CreateSharedRef<Triangle>(mesh, i);
        lights.push_back(CreateSharedRef<AreaLight>(tri));
        accel.Add(tri);
    }
}

inline void Scene::Reset()
{
    accel.Reset();
    lights.clear();
}

inline void Scene::Rebuild()
{
    accel.Rebuild();
}

} // namespace spt