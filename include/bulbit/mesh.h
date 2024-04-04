#pragma once

#include "dynamic_bvh.h"
#include "microfacet.h"

namespace bulbit
{

// Represents triangle mesh
class Mesh
{
public:
    struct Vertex
    {
        Point3 position;
        Vec3 normal;
        Vec3 tangent;
        Point2 texCoord;
    };

    Mesh(std::vector<Point3> positions,
         std::vector<Vec3> normals,
         std::vector<Vec3> tangents,
         std::vector<Point2> texCoords,
         std::vector<int32> indices,
         const Mat4& transform,
         MaterialIndex material);
    Mesh(const std::vector<Vertex>& vertices, std::vector<int32> indices, const Mat4& transform, MaterialIndex material);

    MaterialIndex GetMaterial() const;
    void SetMaterial(MaterialIndex material);

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

    MaterialIndex material;
};

inline MaterialIndex Mesh::GetMaterial() const
{
    return material;
}

inline void Mesh::SetMaterial(MaterialIndex mat)
{
    material = mat;
}

inline int32 Mesh::GetTriangleCount() const
{
    return triangle_count;
}

} // namespace bulbit