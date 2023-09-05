#include "spt/mesh.h"
#include "spt/lambertian.h"
#include "spt/triangle.h"

namespace spt
{

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<u32>& indices,
           const std::array<Ref<Texture>, TextureType::count>& _textures)
    : textures{ _textures }
{
    // std::cout << HasTexture(basecolor) << std::endl;
    // std::cout << HasTexture(normal) << std::endl;
    // std::cout << HasTexture(metallic) << std::endl;
    // std::cout << HasTexture(roughness) << std::endl;
    // std::cout << HasTexture(ao) << std::endl;
    // std::cout << HasTexture(emissive) << std::endl;

    if (HasTexture(basecolor) == false && Material::fallback_material != nullptr)
    {
        material = Material::fallback_material;
    }
    else
    {
        auto mat = CreateSharedRef<Microfacet>();
        mat->basecolor_map = HasTexture(basecolor) ? textures[basecolor] : SolidColor::Create(1.0, 0.0, 1.0);
        mat->normal_map = HasTexture(normal) ? textures[normal] : SolidColor::Create(0.5, 0.5, 1.0);
        mat->metallic_map = HasTexture(metallic) ? textures[metallic] : SolidColor::Create(0.0, 0.0, 0.0);
        mat->roughness_map = HasTexture(roughness) ? textures[roughness] : SolidColor::Create(0.2, 0.2, 0.2);
        mat->ao_map = HasTexture(ao) ? textures[ao] : SolidColor::Create(1.0, 1.0, 1.0);
        mat->emissive_map = HasTexture(emissive) ? textures[emissive] : SolidColor::Create(0.0, 0.0, 0.0);
        material = mat;
    }

    // Bake BVH

    triangles.reserve(indices.size() / 3 + 1);

    for (size_t i = 0; i < indices.size(); i += 3)
    {
        u32 index0 = indices[i];
        u32 index1 = indices[i + 1];
        u32 index2 = indices[i + 2];

        Triangle& t = triangles.emplace_back(vertices[index0], vertices[index1], vertices[index2], material);
        AABB aabb;
        t.GetAABB(&aabb);
        bvh.CreateNode(&t, aabb);
    }
}

} // namespace spt