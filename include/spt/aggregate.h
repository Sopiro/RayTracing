#pragma once

#include "bvh.h"
#include "model.h"
#include "triangle.h"

namespace spt
{

// Represents aggregate primitive
class Aggregate : public Intersectable
{
public:
    virtual ~Aggregate() = default;

    virtual bool Intersect(Intersection* out_is, const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual bool IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual void GetAABB(AABB* out_aabb) const override;

    void Add(const Ref<Intersectable> object);
    void Add(const Ref<Mesh> mesh);
    void Add(const Ref<Model> model);

    void Reset();
    void Rebuild();

private:
    BVH bvh;

    std::vector<Ref<Intersectable>> objects;
};

inline void Aggregate::GetAABB(AABB* out_aabb) const
{
    if (bvh.nodeCount == 0)
    {
        out_aabb->min.SetZero();
        out_aabb->max.SetZero();
    }

    *out_aabb = bvh.nodes[bvh.root].aabb;
}

inline void Aggregate::Add(const Ref<Intersectable> object)
{
    objects.push_back(object);

    AABB aabb;
    object->GetAABB(&aabb);
    bvh.CreateNode(object.get(), aabb);
}

inline void Aggregate::Add(const Ref<Mesh> mesh)
{
    for (i32 i = 0; i < mesh->triangle_count; ++i)
    {
        auto tri = CreateSharedRef<Triangle>(mesh, i);
        objects.push_back(tri);

        AABB aabb;
        tri->GetAABB(&aabb);
        bvh.CreateNode(tri.get(), aabb);
    }
}

inline void Aggregate::Add(const Ref<Model> model)
{
    auto& meshes = model->GetMeshes();

    for (size_t i = 0; i < meshes.size(); ++i)
    {
        Add(meshes[i]);
    }
}

inline void Aggregate::Reset()
{
    bvh.Reset();
}

inline void Aggregate::Rebuild()
{
    bvh.Rebuild();
}

} // namespace spt
