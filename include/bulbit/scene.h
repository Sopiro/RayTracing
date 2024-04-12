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
    Scene() = default;
    virtual ~Scene() = default;

    void GetAABB(AABB* out_aabb) const;
    bool Intersect(Intersection* out_is, const Ray& ray, Float t_min, Float t_max) const;
    bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const;

    void AddPrimitive(const Ref<Primitive> primitive);
    void AddMesh(const Ref<Mesh> mesh);
    void AddModel(Model& model);

    template <typename T, typename... Args>
    void CreateLight(Args&&... args);
    void AddLight(const Ref<Light> light);

    template <typename T, typename... Args>
    MaterialIndex CreateMaterial(Args&&... args);
    MaterialIndex AddMaterial(const Ref<Material> material);

    const Material* GetMaterial(MaterialIndex material) const;
    Material* GetMaterial(MaterialIndex material);

    const std::vector<Ref<Primitive>>& GetPrimitives() const;

    const std::vector<Ref<Light>>& GetLights() const;
    const std::vector<InfiniteAreaLight*>& GetInfiniteAreaLights() const;

    void BuildAccelerationStructure();
    void Clear();

private:
    // Acceleration structure
    std::unique_ptr<Intersectable> accel;

    // All primitives in this scene
    std::vector<Ref<Primitive>> primitives;
    std::vector<Ref<Material>> materials;

    std::vector<Ref<Light>> lights;
    std::vector<InfiniteAreaLight*> infinite_lights;
};

inline void Scene::GetAABB(AABB* out_aabb) const
{
    accel->GetAABB(out_aabb);
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
    materials.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
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

inline const std::vector<Ref<Primitive>>& Scene::GetPrimitives() const
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