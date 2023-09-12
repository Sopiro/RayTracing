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
    auto& vertices = mesh->vertices;
    auto& indices = mesh->indices;

    for (size_t i = 0; i < mesh->indices.size(); i += 3)
    {
        u32 index0 = indices[i];
        u32 index1 = indices[i + 1];
        u32 index2 = indices[i + 2];

        Vertex& vertex0 = vertices[index0];
        Vertex& vertex1 = vertices[index1];
        Vertex& vertex2 = vertices[index2];

        auto tri = CreateSharedRef<Triangle>(vertex0, vertex1, vertex2, mesh->material);

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
