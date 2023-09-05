#pragma once

#include "bvh.h"
#include "common.h"
#include "directional_light.h"
#include "image_texture.h"
#include "intersectable.h"
#include "intersectable_list.h"
#include "solid_color.h"
#include "sphere.h"

namespace spt
{

class Scene : public Intersectable
{
public:
    Scene();

    void Reset();
    void Add(const Ref<Intersectable>& object);
    void AddLight(const Ref<Intersectable>& object);

    virtual bool Intersect(Intersection* out_is, const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual bool GetAABB(AABB* out_aabb) const override;
    virtual f64 EvaluatePDF(const Ray& ray) const override;
    virtual Vec3 GetRandomDirection(const Point3& origin) const override;
    virtual void Rebuild() override;

    const IntersectableList& GetIntersectableList() const;

    bool HasAreaLights() const;
    const IntersectableList& GetAreaLights() const;

    const Ref<Texture>& GetEnvironmentMap() const;
    void SetEnvironmentMap(const Ref<Texture> color);
    Color GetSkyColor(Vec3 direction) const;

    bool HasDirectionalLight() const;
    const Ref<DirectionalLight>& GetDirectionalLight() const;
    void SetDirectionalLight(const Ref<DirectionalLight>& directional_light);

private:
    IntersectableList intersectables;
    IntersectableList lights;

    Ref<Texture> environment_map;
    Ref<DirectionalLight> directional_light;
};

inline Scene::Scene()
    : environment_map{ SolidColor::Create(Color{ 0.0, 0.0, 0.0 }) }
    , directional_light{ nullptr }
{
}

inline void Scene::Reset()
{
    intersectables.Clear();
    lights.Clear();
}

inline void Scene::Add(const Ref<Intersectable>& object)
{
    intersectables.Add(object);
}

inline void Scene::AddLight(const Ref<Intersectable>& object)
{
    lights.Add(object);
}

inline void Scene::Rebuild()
{
    intersectables.Rebuild();
}

inline bool Scene::Intersect(Intersection* is, const Ray& ray, f64 t_min, f64 t_max) const
{
    return intersectables.Intersect(is, ray, t_min, t_max);
}

inline bool Scene::GetAABB(AABB* out_aabb) const
{
    return intersectables.GetAABB(out_aabb);
}

inline f64 Scene::EvaluatePDF(const Ray& ray) const
{
    return intersectables.EvaluatePDF(ray);
}

inline Vec3 Scene::GetRandomDirection(const Point3& origin) const
{
    return intersectables.GetRandomDirection(origin);
}

inline const IntersectableList& Scene::GetIntersectableList() const
{
    return intersectables;
}

inline bool Scene::HasAreaLights() const
{
    return lights.GetCount() > 0;
}

inline const IntersectableList& Scene::GetAreaLights() const
{
    return lights;
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

} // namespace spt