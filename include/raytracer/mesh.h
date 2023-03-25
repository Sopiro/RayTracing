#pragma once

#include "bvh.h"
#include "common.h"
#include "material.h"
#include "pbr_material.h"
#include "triangle.h"

namespace spt
{

enum TextureType
{
    albedo = 0,
    normal,
    roughness,
    metalness,
    ao,
    count
};

class Mesh : public Hittable
{
public:
    Mesh(std::vector<Vertex> vertices,
         std::vector<uint32> indices,
         std::array<std::shared_ptr<Texture>, 5> textures,
         const Mat4& transform);

    virtual bool Hit(const Ray& ray, Real t_min, Real t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;

    bool HasAlbedoTexture() const;
    bool HasNormalTexture() const;
    bool HasRoughnessTexture() const;
    bool HasMetalnessTexture() const;
    bool HasAOTexture() const;

private:
    Mat4 transform;

    BVH bvh;

    // Don't need to hold vertices and indices after baking BVH
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;

    std::vector<Triangle> triangles;
    std::shared_ptr<PBRMaterial> material;
    std::array<std::shared_ptr<Texture>, 5> textures;
};

inline bool Mesh::Hit(const Ray& ray, Real t_min, Real t_max, HitRecord& rec) const
{
    return bvh.Hit(ray, t_min, t_max, rec);
}

inline bool Mesh::GetAABB(AABB& outAABB) const
{
    return bvh.GetAABB(outAABB);
}

inline bool Mesh::HasAlbedoTexture() const
{
    return textures[albedo] != nullptr;
}

inline bool Mesh::HasNormalTexture() const
{
    return textures[normal] != nullptr;
}

inline bool Mesh::HasRoughnessTexture() const
{
    return textures[roughness] != nullptr;
}

inline bool Mesh::HasMetalnessTexture() const
{
    return textures[metalness] != nullptr;
}

inline bool Mesh::HasAOTexture() const
{
    return textures[ao] != nullptr;
}

} // namespace spt