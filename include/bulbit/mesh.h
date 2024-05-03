#pragma once

#include "intersectable.h"
#include "material.h"
#include "microfacet.h"

namespace bulbit
{

struct Vertex
{
    Point3 position;
    Vec3 normal;
    Vec3 tangent;
    Point2 texCoord;
};

// Represents triangle mesh
class Mesh
{
public:
    Mesh(std::vector<Point3> positions,
         std::vector<Vec3> normals,
         std::vector<Vec3> tangents,
         std::vector<Point2> texCoords,
         std::vector<int32> indices,
         const Material* material,
         const Mat4& transform);
    Mesh(const std::vector<Vertex>& vertices, std::vector<int32> indices, const Material* material, const Mat4& transform);

    const Material* GetMaterial() const;
    void SetMaterial(const Material* material);

    int32 GetTriangleCount() const;

private:
    friend class Scene;
    friend class Triangle;

    int32 triangle_count;
    std::vector<Point3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec3> tangents;
    std::vector<Point2> texCoords;
    std::vector<int32> indices;

    const Material* material;
};

inline const Material* Mesh::GetMaterial() const
{
    return material;
}

inline void Mesh::SetMaterial(const Material* mat)
{
    material = mat;
}

inline int32 Mesh::GetTriangleCount() const
{
    return triangle_count;
}

} // namespace bulbit