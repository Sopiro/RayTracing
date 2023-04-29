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
    metallic,
    roughness,
    ao,
    emissive,
    count
};

class Mesh : public Hittable
{
public:
    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<uint32>& indices,
         const std::array<Ref<Texture>, TextureType::count>& textures);

    virtual bool Hit(const Ray& ray, Real t_min, Real t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;
    virtual int32 GetSize() const override;

    bool HasBaseColorTexture() const;
    bool HasNormalTexture() const;
    bool HasRoughnessTexture() const;
    bool HasMetallicTexture() const;
    bool HasAOTexture() const;
    bool HasEmissiveTexture() const;

    const Ref<Material>& GetMaterial() const;
    void SetMaterial(const Ref<Material>& material);

private:
    BVH bvh;
    
    std::vector<Triangle> triangles;
    Ref<Material> material;
    std::array<Ref<Texture>, TextureType::count> textures;
};

inline bool Mesh::Hit(const Ray& ray, Real t_min, Real t_max, HitRecord& rec) const
{
    return bvh.Hit(ray, t_min, t_max, rec);
}

inline bool Mesh::GetAABB(AABB& outAABB) const
{
    return bvh.GetAABB(outAABB);
}

inline int32 Mesh::GetSize() const
{
    return bvh.GetSize();
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

inline const Ref<Material>& Mesh::GetMaterial() const
{
    return material;
}

inline void Mesh::SetMaterial(const Ref<Material>& mat)
{
    for (int32 i = 0; i < triangles.size(); ++i)
    {
        triangles[i].material = mat;
    }
    material = mat;
}

} // namespace spt