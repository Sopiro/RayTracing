#pragma once

#include "bvh.h"
#include "model.h"

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

    void Add(Ref<Intersectable> object);
    void Add(Ref<Mesh> mesh);
    void Add(Ref<Model> model);

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

inline void Aggregate::Add(Ref<Intersectable> object)
{
    objects.push_back(object);

    AABB aabb;
    object->GetAABB(&aabb);
    bvh.CreateNode(object.get(), aabb);
}

inline void Aggregate::Add(Ref<Mesh> mesh)
{
    for (size_t i = 0; i < mesh->indices.size(); i += 3)
    {
        u32 index0 = mesh->indices[i];
        u32 index1 = mesh->indices[i + 1];
        u32 index2 = mesh->indices[i + 2];

        auto tri =
            CreateSharedRef<Triangle>(mesh->vertices[index0], mesh->vertices[index1], mesh->vertices[index2], mesh->material);

        Add(tri);
    }
}

inline void Aggregate::Add(Ref<Model> model)
{
    const std::vector<Ref<Mesh>>& meshes = model->GetMeshes();

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
