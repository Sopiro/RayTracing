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

    const Ref<Material>& GetMaterial() const;
    void SetMaterial(const Ref<Material>& material);

    bool HasTexture(TextureType type) const;

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

inline bool Mesh::HasTexture(TextureType type) const
{
    return textures[type] != nullptr;
}

} // namespace spt