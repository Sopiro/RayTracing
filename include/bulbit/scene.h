#pragma once

#include "bvh.h"
#include "dynamic_bvh.h"

#include "constant_color.h"
#include "image_texture.h"
#include "intersectable.h"

#include "light.h"

namespace bulbit
{

class Scene
{
public:
    Scene();
    ~Scene() noexcept;

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    AABB GetAABB() const;
    bool Intersect(Intersection* out_is, const Ray& ray, Float t_min, Float t_max) const;
    bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const;

    template <typename T, typename... Args>
    void CreatePrimitive(Args&&... args);

    template <typename T, typename... Args>
    void CreateLight(Args&&... args);

    template <typename T, typename... Args>
    const Material* CreateMaterial(Args&&... args);

    const std::vector<Primitive*>& GetPrimitives() const;
    const std::vector<Light*>& GetLights() const;
    const std::vector<Light*>& GetInfiniteLights() const;

    void BuildAccelerationStructure();

private:
    Resource resource;
    PoolResource pool;
    Allocator allocator;

    // Acceleration structure
    std::unique_ptr<Intersectable> accel;

    std::vector<Primitive*> primitives;
    std::vector<Material*> materials;
    std::vector<Light*> lights;
    std::vector<Light*> infinite_lights;
};

inline AABB Scene::GetAABB() const
{
    return accel->GetAABB();
}

inline bool Scene::Intersect(Intersection* out_is, const Ray& ray, Float t_min, Float t_max) const
{
    return accel->Intersect(out_is, ray, t_min, t_max);
}

inline bool Scene::IntersectAny(const Ray& ray, Float t_min, Float t_max) const
{
    return accel->IntersectAny(ray, t_min, t_max);
}

template <typename T, typename... Args>
inline void Scene::CreatePrimitive(Args&&... args)
{
    Primitive* p = allocator.new_object<T>(std::forward<Args>(args)...);
    primitives.push_back(p);

    const Material* mat = p->GetMaterial();
    if (mat->IsLightSource())
    {
        // Create area light
        AreaLight* area_light = allocator.new_object<AreaLight>(p);
        area_light->material = mat;
        lights.push_back(area_light);
    }
}

template <typename T, typename... Args>
inline void Scene::CreateLight(Args&&... args)
{
    Light* light = allocator.new_object<T>(std::forward<Args>(args)...);

    assert(light->type != Light::Type::area_light);

    if (light->type != Light::Type::area_light)
    {
        lights.push_back(light);
    }

    if (light->type == Light::Type::infinite_light)
    {
        infinite_lights.push_back(light);
    }
}

template <typename T, typename... Args>
inline const Material* Scene::CreateMaterial(Args&&... args)
{
    Material* m = allocator.new_object<T>(std::forward<Args>(args)...);
    materials.push_back(m);
    return m;
}

inline const std::vector<Primitive*>& Scene::GetPrimitives() const
{
    return primitives;
}

inline const std::vector<Light*>& Scene::GetLights() const
{
    return lights;
}

inline const std::vector<Light*>& Scene::GetInfiniteLights() const
{
    return infinite_lights;
}

} // namespace bulbit