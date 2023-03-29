#pragma once

#include "bvh.h"
#include "common.h"
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
    void Add(std::shared_ptr<Hittable> object);
    void AddLight(std::shared_ptr<Hittable> object);

    virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;
    virtual double EvaluatePDF(const Ray& ray) const override;
    virtual Vec3 GetRandomDirection(const Vec3& origin) const override;
    virtual void Rebuild() override;

    const HittableList& GetHittableList() const;
    const HittableList& GetLights() const;

    bool HasLights() const;

    const std::shared_ptr<Texture> GetEnvironmentMap() const;
    void SetEnvironmentMap(const std::shared_ptr<Texture> color);

    Vec3 GetSkyColor(Vec3 direction) const;

private:
    HittableList hittables;
    HittableList lights;

    std::shared_ptr<Texture> environment_map;
};

inline Scene::Scene()
{
    // environment_map = ImageTexture::Create("res/sunflowers/sunflowers_4k.hdr", false, true);
    // environment_map = ImageTexture::Create("res/sunflowers/sunflowers_puresky_4k.hdr", false, true);

    environment_map = SolidColor::Create(Color{ 0.7, 0.8, 0.9 });
}

inline void Scene::Reset()
{
    hittables.Clear();
    lights.Clear();
}

inline void Scene::Add(std::shared_ptr<Hittable> object)
{
    hittables.Add(object);
}

inline void Scene::AddLight(std::shared_ptr<Hittable> object)
{
    lights.Add(object);
}

inline void Scene::Rebuild()
{
    hittables.Rebuild();
}

inline bool Scene::Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const
{
    return hittables.Hit(ray, t_min, t_max, rec);
}

inline bool Scene::GetAABB(AABB& outAABB) const
{
    return hittables.GetAABB(outAABB);
}

inline double Scene::EvaluatePDF(const Ray& ray) const
{
    return hittables.EvaluatePDF(ray);
}

inline Vec3 Scene::GetRandomDirection(const Vec3& origin) const
{
    return hittables.GetRandomDirection(origin);
}

inline const HittableList& Scene::GetHittableList() const
{
    return hittables;
}

inline const HittableList& Scene::GetLights() const
{
    return lights;
}

inline bool Scene::HasLights() const
{
    return lights.GetCount() > 0;
}

inline const std::shared_ptr<Texture> Scene::GetEnvironmentMap() const
{
    return environment_map;
}

inline void Scene::SetEnvironmentMap(const std::shared_ptr<Texture> env_map)
{
    environment_map = env_map;
}

inline Vec3 Scene::GetSkyColor(Vec3 dir) const
{
    dir.Normalize();

    double phi = atan2(-dir.z, dir.x) + pi;
    double theta = acos(-dir.y);

    double u = phi / (2.0 * pi);
    double v = theta / pi;

    return environment_map->Value(UV{ u, v }, zero_vec3);
}

} // namespace spt