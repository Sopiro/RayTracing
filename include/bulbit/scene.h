#pragma once

#include "bvh.h"
#include "dynamic_bvh.h"

#include "constant_color.h"
#include "image_texture.h"
#include "intersectable.h"
#include "model.h"

#include "area_light.h"
#include "directional_light.h"
#include "infinite_area_light.h"
#include "point_light.h"

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
    void AddPrimitive(const std::unique_ptr<Primitive> primitive);

    void AddMesh(const Ref<Mesh> mesh);
    void AddModel(const Model& model);

    template <typename T, typename... Args>
    void CreateLight(Args&&... args);
    void AddLight(const Ref<Light> light);

    template <typename T, typename... Args>
    MaterialIndex CreateMaterial(Args&&... args);
    MaterialIndex AddMaterial(const Ref<Material> material);

    const Material* GetMaterial(MaterialIndex material) const;
    Material* GetMaterial(MaterialIndex material);

    const std::vector<Primitive*>& GetPrimitives() const;

    const std::vector<Ref<Light>>& GetLights() const;
    const std::vector<InfiniteAreaLight*>& GetInfiniteAreaLights() const;

    void BuildAccelerationStructure();
    void Clear();

private:
    Resource resource;
    PoolResource pool;
    Allocator allocator;

    // Acceleration structure
    std::unique_ptr<Intersectable> accel;

    // All primitives in this scene
    std::vector<Primitive*> primitives;
    std::vector<Ref<Material>> materials;

    std::vector<Ref<Light>> lights;
    std::vector<InfiniteAreaLight*> infinite_lights;
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

    const Material* mat = GetMaterial(p->GetMaterialIndex());
    if (mat->IsLightSource())
    {
        // Create area light
        auto area_light = std::make_shared<AreaLight>(p);
        area_light->material = mat;
        lights.push_back(area_light);
    }
}

template <typename T, typename... Args>
inline void Scene::CreateLight(Args&&... args)
{
    auto light = std::make_shared<T>(std::forward<Args>(args)...);

    assert(light->type != Light::Type::area_light);

    if (light->type != Light::Type::area_light)
    {
        lights.push_back(light);
    }

    if (light->type == Light::Type::infinite_area_light)
    {
        infinite_lights.push_back((InfiniteAreaLight*)light.get());
    }
}

template <typename T, typename... Args>
inline MaterialIndex Scene::CreateMaterial(Args&&... args)
{
    Ref<Material> m = std::make_shared<T>(std::forward<Args>(args)...);
    materials.push_back(m);
    return MaterialIndex(materials.size() - 1);
}

inline MaterialIndex Scene::AddMaterial(const Ref<Material> material)
{
    materials.push_back(material);
    return MaterialIndex(materials.size() - 1);
}

inline const Material* Scene::GetMaterial(MaterialIndex material) const
{
    return materials[material].get();
}

inline Material* Scene::GetMaterial(MaterialIndex material)
{
    return materials[material].get();
}

inline const std::vector<Primitive*>& Scene::GetPrimitives() const
{
    return primitives;
}

inline const std::vector<Ref<Light>>& Scene::GetLights() const
{
    return lights;
}

inline const std::vector<InfiniteAreaLight*>& Scene::GetInfiniteAreaLights() const
{
    return infinite_lights;
}

} // namespace bulbit