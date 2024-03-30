#pragma once

#include "aggregate.h"
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

    void Add(const Ref<Intersectable> object);
    void Add(const Ref<Mesh> object);
    void Add(const Ref<Model> object);

    void AddLight(const Ref<Light> light);
    void AddLight(const Ref<Primitive> object);
    void AddLight(const Ref<Mesh> object);

    const std::vector<Ref<Light>>& GetLights() const;
    const std::vector<InfiniteAreaLight*>& GetInfiniteAreaLights() const;

    void Reset();
    void Rebuild();

private:
    DynamicBVH bvh; // Acceleration structure
    std::vector<Ref<Intersectable>> objects;

    std::vector<Ref<Light>> lights;
    std::vector<InfiniteAreaLight*> infinite_lights;
};

inline const std::vector<Ref<Light>>& Scene::GetLights() const
{
    return lights;
}

inline const std::vector<InfiniteAreaLight*>& Scene::GetInfiniteAreaLights() const
{
    return infinite_lights;
}

inline void Scene::Reset()
{
    bvh.Reset();
    objects.clear();
    lights.clear();
    infinite_lights.clear();
}

inline void Scene::Rebuild()
{
    bvh.Rebuild();
}

} // namespace bulbit