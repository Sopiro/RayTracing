#pragma once

#include "bvh.h"
#include "common.h"
#include "directional_light.h"
#include "hittable.h"
#include "hittable_list.h"
#include "image_texture.h"
#include "solid_color.h"
#include "sphere.h"

namespace spt
{

class Scene : public Hittable
{
public:
    Scene();

    void Reset();
    void Add(const Ref<Hittable>& object);
    void AddLight(const Ref<Hittable>& object);

    virtual bool Hit(const Ray& ray, float64 t_min, float64 t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;
    virtual float64 EvaluatePDF(const Ray& ray) const override;
    virtual Vec3 GetRandomDirection(const Point3& origin) const override;
    virtual void Rebuild() override;

    const HittableList& GetHittableList() const;

    bool HasLights() const;
    const HittableList& GetLights() const;

    const Ref<Texture>& GetEnvironmentMap() const;
    void SetEnvironmentMap(const Ref<Texture> color);
    Color GetSkyColor(Vec3 direction) const;

    bool HasDirectionalLight() const;
    const Ref<DirectionalLight>& GetDirectionalLight() const;
    void SetDirectionalLight(const Ref<DirectionalLight>& directional_light);

private:
    HittableList hittables;
    HittableList lights;

    Ref<Texture> environment_map;
    Ref<DirectionalLight> directional_light;
};

inline Scene::Scene()
{
    // environment_map = ImageTexture::Create("res/sunflowers/sunflowers_4k.hdr", false, true);
    // environment_map = ImageTexture::Create("res/sunflowers/sunflowers_puresky_4k.hdr", false, true);

    environment_map = SolidColor::Create(Color{ 0.7, 0.8, 0.9 });
    directional_light = nullptr;
}

inline void Scene::Reset()
{
    hittables.Clear();
    lights.Clear();
}

inline void Scene::Add(const Ref<Hittable>& object)
{
    hittables.Add(object);
}

inline void Scene::AddLight(const Ref<Hittable>& object)
{
    lights.Add(object);
}

inline void Scene::Rebuild()
{
    hittables.Rebuild();
}

inline bool Scene::Hit(const Ray& ray, float64 t_min, float64 t_max, HitRecord& rec) const
{
    return hittables.Hit(ray, t_min, t_max, rec);
}

inline bool Scene::GetAABB(AABB& outAABB) const
{
    return hittables.GetAABB(outAABB);
}

inline float64 Scene::EvaluatePDF(const Ray& ray) const
{
    return hittables.EvaluatePDF(ray);
}

inline Vec3 Scene::GetRandomDirection(const Point3& origin) const
{
    return hittables.GetRandomDirection(origin);
}

inline const HittableList& Scene::GetHittableList() const
{
    return hittables;
}

inline bool Scene::HasLights() const
{
    return lights.GetCount() > 0;
}

inline const HittableList& Scene::GetLights() const
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

    float64 phi = atan2(-dir.z, dir.x) + pi;
    float64 theta = acos(-dir.y);

    float64 u = phi * inv_two_pi;
    float64 v = theta * inv_pi;

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