#pragma once

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

class Scene : public Intersectable
{
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual bool Intersect(Intersection* out_is, const Ray& ray, Float t_min, Float t_max) const override;
    virtual bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const override;
    virtual void GetAABB(AABB* out_aabb) const override;

    void Add(const Ref<Primitive> primitive);
    void Add(const Ref<Mesh> mesh);
    void Add(const Ref<Model> model);

    void AddLight(const Ref<Light> light);
    void AddLight(const Ref<Primitive> primitve);
    void AddLight(const Ref<Mesh> mesh);

    template <typename T, typename... Args>
    MaterialIndex CreateMaterial(Args&&... args);
    MaterialIndex Add(const Ref<Material> material);

    const Material* GetMaterial(MaterialIndex material) const;
    Material* GetMaterial(MaterialIndex material);

    const std::vector<Ref<Primitive>>& GetPrimitives() const;

    const std::vector<Ref<Light>>& GetLights() const;
    const std::vector<InfiniteAreaLight*>& GetInfiniteAreaLights() const;

    void BuildAccelerationStructure();
    void Clear();

private:
    // Acceleration structure
    DynamicBVH bvh;

    // All primitives in this scene
    std::vector<Ref<Primitive>> primitives;
    std::vector<Ref<Material>> materials;

    std::vector<Ref<Light>> lights;
    std::vector<InfiniteAreaLight*> infinite_lights;
};

template <typename T, typename... Args>
inline MaterialIndex Scene::CreateMaterial(Args&&... args)
{
    materials.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
    return MaterialIndex(materials.size() - 1);
}

inline MaterialIndex Scene::Add(const Ref<Material> material)
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

inline void Scene::BuildAccelerationStructure()
{
    bvh.Rebuild();
}

inline void Scene::Clear()
{
    bvh.Reset();
    primitives.clear();
    lights.clear();
    infinite_lights.clear();
}

} // namespace bulbit