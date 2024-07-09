#pragma once

#include "light.h"
#include "material.h"
#include "mesh.h"

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
    T* CreateMaterial(Args&&... args);
    template <typename T, typename... Args>
    T* CreateLight(Args&&... args);
    template <typename T, typename... Args>
    T* CreateShape(Args&&... args);
    template <typename T, typename... Args>
    T* CreatePrimitive(Args&&... args);
    template <typename... Args>
    Mesh* CreateMesh(Args&&... args);

    const std::vector<Primitive*>& GetPrimitives() const;
    const std::vector<Light*>& GetLights() const;

private:
    Resource resource;
    PoolResource pool;
    Allocator allocator;

    std::vector<Material*> materials;
    std::vector<Light*> lights;
    std::vector<Shape*> shapes;
    std::vector<Primitive*> primitives;
    std::vector<std::unique_ptr<Mesh>> meshes;
};

inline Scene::Scene()
    : resource{ 64 * 1024 }
    , pool{ &resource }
    , allocator{ &pool }
{
}

inline Scene::~Scene() noexcept
{
    // Free all pooled resources

    for (Shape* s : shapes)
    {
        allocator.delete_object(s);
    }

    for (Material* m : materials)
    {
        allocator.delete_object(m);
    }

    for (Primitive* p : primitives)
    {
        allocator.delete_object(p);
    }

    for (Light* l : lights)
    {
        allocator.delete_object(l);
    }
}

template <typename T, typename... Args>
inline T* Scene::CreateMaterial(Args&&... args)
{
    T* m = allocator.new_object<T>(std::forward<Args>(args)...);
    materials.push_back(m);
    return m;
}

template <typename T, typename... Args>
inline T* Scene::CreateLight(Args&&... args)
{
    T* l = allocator.new_object<T>(std::forward<Args>(args)...);
    lights.push_back(l);
    return l;
}

template <typename T, typename... Args>
inline T* Scene::CreateShape(Args&&... args)
{
    T* p = allocator.new_object<T>(std::forward<Args>(args)...);
    shapes.push_back(p);
    return p;
}

template <typename T, typename... Args>
inline T* Scene::CreatePrimitive(Args&&... args)
{
    T* p = allocator.new_object<T>(std::forward<Args>(args)...);
    primitives.push_back(p);
    return p;
}

template <typename... Args>
inline Mesh* Scene::CreateMesh(Args&&... args)
{
    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(std::forward<Args>(args)...);
    Mesh* m = mesh.get();
    meshes.push_back(std::move(mesh));
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

} // namespace bulbit