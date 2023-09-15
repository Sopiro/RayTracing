#pragma once

#include "bvh.h"
#include "common.h"
#include "microfacet.h"

namespace spt
{

struct Vertex
{
    Point3 position;
    Vec3 normal;
    Vec3 tangent;
    UV texCoord;
};

// Represents triangle mesh
class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<i32> indices, const Mat4& transform, const Ref<Material> material);

    const Material* GetMaterial() const;
    void SetMaterial(const Ref<Material> material);

    i32 GetTriangleCount() const;

private:
    friend class Aggregate;
    friend class Scene;
    friend class Triangle;

    i32 triangle_count;
    std::vector<Vertex> vertices;
    std::vector<i32> indices;

    Ref<Material> material;
};

inline const Material* Mesh::GetMaterial() const
{
    return material.get();
}

inline void Mesh::SetMaterial(const Ref<Material> mat)
{
    material = mat;
}

inline i32 Mesh::GetTriangleCount() const
{
    return triangle_count;
}

} // namespace spt