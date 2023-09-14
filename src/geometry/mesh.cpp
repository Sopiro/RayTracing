#include "spt/mesh.h"
#include "spt/lambertian.h"
#include "spt/triangle.h"

#include "spt/util.h"

namespace spt
{

Mesh::Mesh(const std::vector<Triangle>& triangles, const Ref<Material> _material)
    : material{ _material }
{
    u32 k = 0;
    for (size_t i = 0; i < triangles.size(); ++i)
    {
        const Triangle& tri = triangles[i];

        vertices.push_back(tri.v0);
        vertices.push_back(tri.v1);
        vertices.push_back(tri.v2);

        indices.push_back(k++);
        indices.push_back(k++);
        indices.push_back(k++);
    }
}

Mesh::Mesh(const std::vector<Vertex>& _vertices,
           const std::vector<u32>& _indices,
           const std::array<Ref<Texture>, TextureType::count>& _textures,
           const std::array<Color, 3>& colors,
           const Mat4& transform)
    : vertices{ _vertices }
    , indices{ _indices }
    , textures{ _textures }
{
    // std::cout << "Basecolor\t: " << HasTexture(basecolor) << std::endl;
    // std::cout << "Normal\t\t: " << HasTexture(normal) << std::endl;
    // std::cout << "Metallic\t: " << HasTexture(metallic) << std::endl;
    // std::cout << "Roughness\t: " << HasTexture(roughness) << std::endl;
    // std::cout << "AO\t\t: " << HasTexture(ao) << std::endl;
    // std::cout << "Emissive\t: " << HasTexture(emissive) << std::endl;
    // std::cout << std::endl;

    // Transform vertex attributes with given transform
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        Vertex& v = vertices[i];

        Vec4 vP = Mul(transform, Vec4(v.position, 1.0));
        Vec4 vN = Mul(transform, Vec4(v.normal, 0.0));
        Vec4 vT = Mul(transform, Vec4(v.tangent, 0.0));
        vN.Normalize();
        vT.Normalize();

        v.position.Set(vP.x, vP.y, vP.z);
        v.normal.Set(vN.x, vN.y, vN.z);
        v.tangent.Set(vT.x, vT.y, vT.z);
    }

    if (HasTexture(basecolor) == false && Material::fallback_material != nullptr)
    {
        material = Material::fallback_material;
        return;
    }

    auto mat = CreateSharedRef<Microfacet>();

    if (HasTexture(basecolor))
    {
        mat->basecolor_map = textures[basecolor];
    }
    else
    {
        mat->basecolor_map = SolidColor::Create(colors[0]);
    }

    mat->normal_map = HasTexture(normal) ? textures[normal] : SolidColor::Create(0.5, 0.5, 1.0);

    if (HasTexture(metallic))
    {
        mat->metallic_map = textures[metallic];
    }
    else
    {
        if (colors[1].x < 0.01 && colors[1].y < 0.01 && colors[1].z < 0.01)
        {
            mat->metallic_map = SolidColor::Create(0.0);
        }
        else
        {
            mat->metallic_map = SolidColor::Create(1.0);
        }
    }

    if (HasTexture(roughness))
    {
        mat->roughness_map = textures[roughness];
    }
    else
    {
        mat->roughness_map = SolidColor::Create(colors[1]);
    }

    mat->ao_map = HasTexture(ao) ? textures[ao] : SolidColor::Create(1.0);

    if (HasTexture(emissive))
    {
        mat->emissive_map = textures[emissive];
    }
    else
    {
        mat->emissive_map = SolidColor::Create(colors[2]);
    }

    material = mat;
}

} // namespace spt