#pragma once

#include "bvh.h"
#include "common.h"
#include "material.h"
#include "microfacet.h"
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

class Mesh : public Intersectable
{
public:
    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<u32>& indices,
         const std::array<Ref<Texture>, TextureType::count>& textures);

    virtual bool Intersect(const Ray& ray, Real t_min, Real t_max, Intersection& is) const override;
    virtual bool GetAABB(AABB& out_aabb) const override;
    virtual i32 GetSize() const override;

    virtual const Material* GetMaterial() const override;
    void SetMaterial(const Ref<Material>& material);
    bool HasTexture(TextureType type) const;

private:
    BVH bvh;

    std::vector<Triangle> triangles;
    Ref<Material> material;
    std::array<Ref<Texture>, TextureType::count> textures;
};

inline bool Mesh::Intersect(const Ray& ray, Real t_min, Real t_max, Intersection& is) const
{
    return bvh.Intersect(ray, t_min, t_max, is);
}

inline bool Mesh::GetAABB(AABB& out_aabb) const
{
    return bvh.GetAABB(out_aabb);
}

inline i32 Mesh::GetSize() const
{
    return bvh.GetSize();
}

inline const Material* Mesh::GetMaterial() const
{
    return material.get();
}

inline void Mesh::SetMaterial(const Ref<Material>& mat)
{
    for (i32 i = 0; i < triangles.size(); ++i)
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