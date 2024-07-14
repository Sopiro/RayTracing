#pragma once

#include "intersectable.h"
#include "material.h"
#include "matrix.h"
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
    Mesh(
        std::vector<Point3> positions,
        std::vector<Vec3> normals,
        std::vector<Vec3> tangents,
        std::vector<Point2> texCoords,
        std::vector<int32> indices,
        const Mat4& transform
    );
    Mesh(const std::vector<Vertex>& vertices, std::vector<int32> indices, const Mat4& transform);

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
};

inline int32 Mesh::GetTriangleCount() const
{
    return triangle_count;
}

} // namespace bulbit