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
    basecolor = 0,
    normal,
    roughness,
    metallic,
    ao,
    emissive,
    count
};

class Mesh : public Hittable
{
public:
    Mesh(std::vector<Vertex> vertices,
         std::vector<uint32> indices,
         std::array<std::shared_ptr<Texture>, TextureType::count> textures,
         const Mat4& transform);

    virtual bool Hit(const Ray& ray, Real t_min, Real t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;

    bool HasBaseColorTexture() const;
    bool HasNormalTexture() const;
    bool HasRoughnessTexture() const;
    bool HasMetallicTexture() const;
    bool HasAOTexture() const;
    bool HasEmissiveTexture() const;

private:
    Mat4 transform;

    BVH bvh;

    // Don't need to hold vertices and indices after baking BVH
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;

    std::vector<Triangle> triangles;
    std::shared_ptr<Material> material;
    std::array<std::shared_ptr<Texture>, TextureType::count> textures;
};

inline bool Mesh::Hit(const Ray& ray, Real t_min, Real t_max, HitRecord& rec) const
{
    return bvh.Hit(ray, t_min, t_max, rec);
}

inline bool Mesh::GetAABB(AABB& outAABB) const
{
    return bvh.GetAABB(outAABB);
}

inline bool Mesh::HasBaseColorTexture() const
{
    return textures[basecolor] != nullptr;
}

inline bool Mesh::HasNormalTexture() const
{
    return textures[normal] != nullptr;
}

inline bool Mesh::HasRoughnessTexture() const
{
    return textures[roughness] != nullptr;
}

inline bool Mesh::HasMetallicTexture() const
{
    return textures[metallic] != nullptr;
}

inline bool Mesh::HasAOTexture() const
{
    return textures[ao] != nullptr;
}

inline bool Mesh::HasEmissiveTexture() const
{
    return textures[emissive] != nullptr;
}

} // namespace spt