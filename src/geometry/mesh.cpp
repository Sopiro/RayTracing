#include "bulbit/mesh.h"
#include "bulbit/triangle.h"

#include "bulbit/util.h"

namespace bulbit
{

Mesh::Mesh(std::vector<Point3> _positions,
           std::vector<Vec3> _normals,
           std::vector<Vec3> _tangents,
           std::vector<Point2> _texCoords,
           std::vector<int32> _indices,
           const Material* _material,
           const Mat4& transform)
    : positions{ std::move(_positions) }
    , normals{ std::move(_normals) }
    , tangents{ std::move(_tangents) }
    , texCoords{ std::move(_texCoords) }
    , indices{ std::move(_indices) }
    , material{ _material }
{
    size_t count = positions.size();

    // Transform vertices to the world space
    for (size_t i = 0; i < count; ++i)
    {
        Vec4 vP = Mul(transform, Vec4(positions[i], 1));
        Vec4 vN = Mul(transform, Vec4(normals[i], 0));
        Vec4 vT = Mul(transform, Vec4(tangents[i], 0));
        vN.Normalize();
        vT.Normalize();

        positions[i].Set(vP.x, vP.y, vP.z);
        normals[i].Set(vN.x, vN.y, vN.z);
        tangents[i].Set(vT.x, vT.y, vT.z);
    }

    triangle_count = int32(indices.size() / 3);
}

Mesh::Mesh(const std::vector<Vertex>& vertices, std::vector<int32> _indices, const Material* _material, const Mat4& transform)
    : indices{ std::move(_indices) }
    , material{ _material }
{
    size_t count = vertices.size();

    positions.resize(count);
    normals.resize(count);
    tangents.resize(count);
    texCoords.resize(count);

    // Transform vertices to the world space
    for (size_t i = 0; i < count; ++i)
    {
        Vec4 vP = Mul(transform, Vec4(vertices[i].position, 1));
        Vec4 vN = Mul(transform, Vec4(vertices[i].normal, 0));
        Vec4 vT = Mul(transform, Vec4(vertices[i].texCoord, 0));
        vN.Normalize();
        vT.Normalize();

        positions[i].Set(vP.x, vP.y, vP.z);
        normals[i].Set(vN.x, vN.y, vN.z);
        tangents[i].Set(vT.x, vT.y, vT.z);

        texCoords[i] = vertices[i].texCoord;
    }

    triangle_count = int32(indices.size() / 3);
}

} // namespace bulbit