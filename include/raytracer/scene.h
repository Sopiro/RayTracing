#pragma once

#include "bvh.h"
#include "common.h"
#include "hittable.h"
#include "hittable_list.h"
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

    Color GetSkyColor() const;
    void SetSkyColor(const Color& color);

private:
    Color sky_color;
    HittableList hittables;
    HittableList lights;
};

inline Scene::Scene()
    : sky_color{ 0.0 }
{
}

inline void Scene::Reset()
{
    hittables.Clear();
    lights.Clear();
    sky_color.SetZero();
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

inline Color Scene::GetSkyColor() const
{
    return sky_color;
}

inline void Scene::SetSkyColor(const Color& color)
{
    sky_color = color;
}

} // namespace spt