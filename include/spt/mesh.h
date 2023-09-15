#pragma once

#include "bvh.h"
#include "common.h"
#include "material.h"
#include "microfacet.h"
#include "triangle.h"

namespace spt
{

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<u32> indices, const Mat4& transform, const Ref<Material> material);

    const Material* GetMaterial() const;
    void SetMaterial(const Ref<Material> material);

private:
    friend class Aggregate;
    friend class Scene;

    std::vector<Vertex> vertices;
    std::vector<u32> indices;

    Ref<Material> material;
};

inline const Material* Mesh::GetMaterial() const
{
    return material.get();
}

inline void Mesh::SetMaterial(const Ref<Material> mat)
{
    material = mat;
}

} // namespace spt