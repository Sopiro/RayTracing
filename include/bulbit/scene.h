#pragma once

#include "light.h"
#include "material.h"
#include "medium.h"
#include "mesh.h"
#include "primitive.h"
#include "shapes.h"
#include "textures.h"

namespace bulbit
{

class Scene
{
public:
    Scene();
    ~Scene() noexcept;

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    template <typename ShapeType, typename... Args>
    ShapeType* CreateShape(Args&&... args);
    template <typename PrimitiveType, typename... Args>
    PrimitiveType* CreatePrimitive(Args&&... args);
    template <typename... Args>
    Mesh* CreateMesh(Args&&... args);

    template <typename LightType, typename... Args>
    LightType* CreateLight(Args&&... args);

    template <typename MediumType, typename... Args>
    MediumType* CreateMedium(Args&&... args);

    template <template <typename> class TextureType, typename T, typename... Args>
    TextureType<T>* CreateTexture(Args&&... args);
    template <typename MaterialType, typename... Args>
    MaterialType* CreateMaterial(Args&&... args);

    const std::vector<Primitive*>& GetPrimitives() const;
    const std::vector<Light*>& GetLights() const;

private:
    Resource resource;
    PoolResource pool;
    Allocator allocator;

    std::vector<Shape*> shapes;
    std::vector<Primitive*> primitives;
    std::vector<std::unique_ptr<Mesh>> meshes;

    std::vector<Light*> lights;

    std::vector<Medium*> media;

    TexturePool texture_pool;
    std::vector<Material*> materials;
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

    for (Medium* m : media)
    {
        allocator.delete_object(m);
    }
}

template <typename ShapeType, typename... Args>
inline ShapeType* Scene::CreateShape(Args&&... args)
{
    ShapeType* shape = allocator.new_object<ShapeType>(std::forward<Args>(args)...);
    shapes.push_back(shape);
    return shape;
}

template <typename PrimitiveType, typename... Args>
inline PrimitiveType* Scene::CreatePrimitive(Args&&... args)
{
    PrimitiveType* primitive = allocator.new_object<PrimitiveType>(std::forward<Args>(args)...);
    primitives.push_back(primitive);
    return primitive;
}

template <typename... Args>
inline Mesh* Scene::CreateMesh(Args&&... args)
{
    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(std::forward<Args>(args)...);
    Mesh* ptr = mesh.get();
    meshes.push_back(std::move(mesh));
    return ptr;
}

template <typename LightType, typename... Args>
inline LightType* Scene::CreateLight(Args&&... args)
{
    LightType* light = allocator.new_object<LightType>(std::forward<Args>(args)...);
    lights.push_back(light);
    return light;
}

template <typename MediumType, typename... Args>
inline MediumType* Scene::CreateMedium(Args&&... args)
{
    MediumType* medium = allocator.new_object<MediumType>(std::forward<Args>(args)...);
    media.push_back(medium);
    return medium;
}

template <template <typename> class TextureType, typename T, typename... Args>
inline TextureType<T>* Scene::CreateTexture(Args&&... args)
{
    return texture_pool.CreateTexture<TextureType, T>(std::forward<Args>(args)...);
}

template <typename MaterialType, typename... Args>
inline MaterialType* Scene::CreateMaterial(Args&&... args)
{
    MaterialType* material = allocator.new_object<MaterialType>(std::forward<Args>(args)...);
    materials.push_back(material);
    return material;
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