#pragma once

#include "light.h"
#include "material.h"

namespace bulbit
{

class Scene
{
public:
    Scene();
    ~Scene() noexcept;

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    template <typename T, typename... Args>
    const Material* CreateMaterial(Args&&... args);

    template <typename T, typename... Args>
    void CreatePrimitive(Args&&... args);

    template <typename T, typename... Args>
    void CreateLight(Args&&... args);

    const std::vector<Primitive*>& GetPrimitives() const;
    const std::vector<Light*>& GetLights() const;

private:
    Resource resource;
    PoolResource pool;
    Allocator allocator;

    std::vector<Material*> materials;
    std::vector<Primitive*> primitives;
    std::vector<Light*> lights;
};

inline Scene::Scene()
    : resource{ 64 * 1024 }
    , pool{ &resource }
    , allocator{ &pool }
{
}

inline Scene::~Scene()
{
    // Free all pooled resources

    for (Primitive* p : primitives)
    {
        allocator.delete_object(p);
    }

    for (Material* m : materials)
    {
        allocator.delete_object(m);
    }

    for (Light* l : lights)
    {
        allocator.delete_object(l);
    }
}

template <typename T, typename... Args>
inline const Material* Scene::CreateMaterial(Args&&... args)
{
    Material* m = allocator.new_object<T>(std::forward<Args>(args)...);
    materials.push_back(m);
    return m;
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
}

inline const std::vector<Primitive*>& Scene::GetPrimitives() const
{
    return primitives;
}

inline const std::vector<Light*>& Scene::GetLights() const
{
    return lights;
}

} // namespace bulbit