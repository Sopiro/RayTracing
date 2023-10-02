#pragma once

#include "dynamic_bvh.h"
#include "model.h"
#include "triangle.h"

namespace spt
{

// Represents aggregate primitive
class Aggregate : public Primitive
{
public:
    virtual ~Aggregate() = default;

    virtual void GetAABB(AABB* out_aabb) const override;
    virtual bool Intersect(Intersection* out_is, const Ray& ray, Float t_min, Float t_max) const override;
    virtual bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const override;

    virtual void Sample(Intersection* sample, Float* pdf) const override;
    virtual void Sample(Intersection* sample, Float* pdf, Vec3* ref2p, const Point3& ref) const override;

    virtual Float EvaluatePDF(const Ray& ray) const override;
    virtual Float PDFValue(const Intersection& hit_is, const Ray& hit_ray) const override;

    virtual const Material* GetMaterial() const override;

    void Add(const Ref<Primitive> primitive);
    void Add(const Ref<Mesh> mesh);
    void Add(const Ref<Model> model);

    void Reset();
    void Rebuild();

private:
    DynamicBVH bvh;
    std::vector<Ref<Primitive>> primitives;
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

inline void Aggregate::Sample(Intersection* sample, Float* pdf) const
{
    assert(false);
}

inline void Aggregate::Sample(Intersection* sample, Float* pdf, Vec3* ref2p, const Point3& ref) const
{
    size_t count = primitives.size();
    size_t index = std::min(size_t(Rand() * count), count - 1);
    Primitive* object = primitives[index].get();

    object->Sample(sample, pdf, ref2p, ref);

    *pdf = EvaluatePDF(Ray(ref, *ref2p));
}

inline Float Aggregate::PDFValue(const Intersection& hit_is, const Ray& hit_ray) const
{
    assert(false);
    return 0.0;
}

inline const Material* Aggregate::GetMaterial() const
{
    assert(false);
    return nullptr;
}

inline void Aggregate::Add(const Ref<Primitive> primitive)
{
    primitives.push_back(primitive);

    AABB aabb;
    primitive->GetAABB(&aabb);
    bvh.CreateNode(primitive.get(), aabb);
}

inline void Aggregate::Add(const Ref<Mesh> mesh)
{
    for (int32 i = 0; i < mesh->triangle_count; ++i)
    {
        auto tri = CreateSharedRef<Triangle>(mesh, i);
        primitives.push_back(tri);

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
    primitives.clear();
}

inline void Aggregate::Rebuild()
{
    bvh.Rebuild();
}

} // namespace spt
