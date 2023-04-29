#include "spt/mesh.h"
#include "spt/lambertian.h"
#include "spt/triangle.h"

namespace spt
{

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<uint32>& indices,
           const std::array<Ref<Texture>, TextureType::count>& _textures)
    : textures{ _textures }
{
    // std::cout << HasBaseColorTexture() << std::endl;
    // std::cout << HasNormalTexture() << std::endl;
    // std::cout << HasRoughnessTexture() << std::endl;
    // std::cout << HasMetallicTexture() << std::endl;
    // std::cout << HasAOTexture() << std::endl;
    // std::cout << HasEmissiveTexture() << std::endl;

    if (HasBaseColorTexture() == false && Material::fallback_material != nullptr)
    {
        material = Material::fallback_material;
    }
    else
    {
        auto mat = CreateSharedRef<PBRMaterial>();
        mat->basecolor_map = HasBaseColorTexture() ? textures[basecolor] : SolidColor::Create(1.0, 0.0, 1.0);
        mat->normal_map = HasNormalTexture() ? textures[normal] : SolidColor::Create(0.5, 0.5, 1.0);
        mat->metallic_map = HasMetallicTexture() ? textures[metallic] : SolidColor::Create(0.0, 0.0, 0.0);
        mat->roughness_map = HasRoughnessTexture() ? textures[roughness] : SolidColor::Create(0.2, 0.2, 0.2);
        mat->ao_map = HasAOTexture() ? textures[ao] : SolidColor::Create(1.0, 1.0, 1.0);
        mat->emissive_map = HasEmissiveTexture() ? textures[emissive] : SolidColor::Create(0.0, 0.0, 0.0);
        material = mat;
    }

    // Bake BVH

    triangles.reserve(indices.size() / 3 + 1);

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        uint32 index0 = indices[i];
        uint32 index1 = indices[i + 1];
        uint32 index2 = indices[i + 2];

        Triangle& t = triangles.emplace_back(vertices[index0], vertices[index1], vertices[index2], material);
        AABB aabb;
        t.GetAABB(aabb);
        bvh.CreateNode(&t, aabb);
    }
}

} // namespace spt