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

class Mesh
{
public:
    Mesh(const std::vector<Triangle>& triangles, const Ref<Material>& material);
    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<u32>& indices,
         const std::array<Ref<Texture>, TextureType::count>& textures,
         const std::array<Color, 3>& colors,
         const Mat4& transform);

    const Material* GetMaterial() const;
    void SetMaterial(const Ref<Material>& material);

    bool HasTexture(TextureType type) const;
    const Ref<Texture> GetTexture(TextureType type) const;

private:
    friend class Aggregate;
    friend class Scene;

    std::vector<Vertex> vertices;
    std::vector<u32> indices;

    std::array<Ref<Texture>, TextureType::count> textures;
    Ref<Material> material;
};

inline const Material* Mesh::GetMaterial() const
{
    return material.get();
}

inline void Mesh::SetMaterial(const Ref<Material>& mat)
{
    material = mat;
}

inline bool Mesh::HasTexture(TextureType type) const
{
    return textures[type] != nullptr;
}

inline const Ref<Texture> Mesh::GetTexture(TextureType type) const
{
    return textures[type];
}

} // namespace spt