#include "raytracer/mesh.h"
#include "raytracer/lambertian.h"
#include "raytracer/triangle.h"

namespace spt
{

#if HOLD_VERTICES_INDICES
Mesh::Mesh(std::vector<Vertex> _vertices,
           std::vector<uint32> _indices,
           std::array<std::shared_ptr<Texture>, 5> _textures,
           const Mat4& _transform)
    : vertices{ std::move(_vertices) }
    , indices{ std::move(_indices) }
    , textures{ std::move(_textures) }
    , transform{ _transform }
{
    std::shared_ptr<Material> mat;

    if (HasAlbedoTexture())
    {
        mat = std::make_shared<Lambertian>(textures[0]);
    }
    else
    {
        mat = std::make_shared<Lambertian>(Color{ 1.0, 0.0, 1.0 });
    }

    for (size_t i = 0; i < vertices.size(); ++i)
    {
        Vertex& v = vertices[i];

        Vec4 vP = transform * Vec4{ v.position, 1.0 };
        Vec4 vN = transform * Vec4{ v.normal, 0.0 };
        vN.Normalize();

        v.position.Set(vP.x, vP.y, vP.z);
        v.normal.Set(vN.x, vN.y, vN.z);
    }

    // Bake BVH

    triangles.reserve(indices.size() / 3 + 1);

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        uint32 index0 = indices[i];
        uint32 index1 = indices[i + 1];
        uint32 index2 = indices[i + 2];

        Triangle& t = triangles.emplace_back(vertices[index0], vertices[index1], vertices[index2], mat, true, false);
        AABB aabb;
        t.GetAABB(aabb);
        bvh.CreateNode(&t, aabb);
    }
}
#else
Mesh::Mesh(std::vector<Vertex> vertices,
           std::vector<uint32> indices,
           std::array<std::shared_ptr<Texture>, 5> _textures,
           const Mat4& _transform)
    : textures{ std::move(_textures) }
    , transform{ _transform }
{
    std::shared_ptr<Material> mat;

    if (HasAlbedoTexture())
    {
        mat = std::make_shared<Lambertian>(textures[0]);
    }
    else
    {
        mat = std::make_shared<Lambertian>(Color{ 1.0, 0.0, 1.0 });
    }

    for (size_t i = 0; i < vertices.size(); ++i)
    {
        Vertex& v = vertices[i];

        Vec4 vP = transform * Vec4{ v.position, 1.0 };
        Vec4 vN = transform * Vec4{ v.normal, 0.0 };
        vN.Normalize();

        v.position.Set(vP.x, vP.y, vP.z);
        v.normal.Set(vN.x, vN.y, vN.z);
    }

    // Bake BVH

    triangles.reserve(indices.size() / 3 + 1);

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        uint32 index0 = indices[i];
        uint32 index1 = indices[i + 1];
        uint32 index2 = indices[i + 2];

        Triangle& t = triangles.emplace_back(vertices[index0], vertices[index1], vertices[index2], mat, true, false);
        AABB aabb;
        t.GetAABB(aabb);
        bvh.CreateNode(&t, aabb);
    }
}
#endif

} // namespace spt