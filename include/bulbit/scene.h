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

    template <typename T, typename... Args>
    T* CreateShape(Args&&... args);
    template <typename T, typename... Args>
    T* CreatePrimitive(Args&&... args);
    template <typename... Args>
    Mesh* CreateMesh(Args&&... args);

    template <typename T, typename... Args>
    T* CreateLight(Args&&... args);

    template <typename T, typename... Args>
    T* CreateMedium(Args&&... args);

    template <typename T, typename... Args>
    ConstantTexture<T>* CreateConstantTexture(T value);
    template <typename T, typename... Args>
    ImageTexture<T>* CreateImageTexture(Image<T> image);
    template <typename T, typename... Args>
    CheckerTexture<T>* CreateCheckerTexture(const Texture<T>* a, const Texture<T>* b, const Point2& resolution);

    template <typename T, typename... Args>
    T* CreateMaterial(Args&&... args);

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

template <typename T, typename... Args>
inline T* Scene::CreateShape(Args&&... args)
{
    T* s = allocator.new_object<T>(std::forward<Args>(args)...);
    shapes.push_back(s);
    return s;
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

template <typename T, typename... Args>
inline T* Scene::CreateLight(Args&&... args)
{
    T* l = allocator.new_object<T>(std::forward<Args>(args)...);
    lights.push_back(l);
    return l;
}

template <typename T, typename... Args>
inline T* Scene::CreateMedium(Args&&... args)
{
    T* m = allocator.new_object<T>(std::forward<Args>(args)...);
    media.push_back(m);
    return m;
}

template <typename T, typename... Args>
inline ConstantTexture<T>* Scene::CreateConstantTexture(T value)
{
    return texture_pool.GetPool<ConstantTexture, T>().Create(value, value);
}

template <typename T, typename... Args>
inline ImageTexture<T>* Scene::CreateImageTexture(Image<T> image)
{
    if (image)
    {
        return texture_pool.GetPool<ImageTexture, T>().Create(
            { &image[0], image.width * image.height }, std::move(image), TexCoordFilter::repeat
        );
    }
    else
    {
        return nullptr;
    }
}

template <typename T, typename... Args>
inline CheckerTexture<T>* Scene::CreateCheckerTexture(const Texture<T>* a, const Texture<T>* b, const Point2& resolution)
{
    return texture_pool.GetPool<CheckerTexture, T>().Create({ a, b, resolution }, a, b, resolution);
}

template <typename T, typename... Args>
inline T* Scene::CreateMaterial(Args&&... args)
{
    T* m = allocator.new_object<T>(std::forward<Args>(args)...);
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

} // namespace bulbit