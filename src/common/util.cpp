#include "spt/util.h"
#include "spt/mesh.h"
#include "spt/triangle.h"

namespace spt
{

Ref<Material> CreateMaterial(const MaterialTextures& textures, const MaterialColors& colors)
{
#define HasTexture(type) (textures.type != nullptr)

    if (HasTexture(basecolor) == false && Material::fallback != nullptr)
    {
        return Material::fallback;
    }

    auto mat = CreateSharedRef<Microfacet>();

    if (HasTexture(basecolor))
    {
        mat->basecolor = textures.basecolor;
    }
    else
    {
        mat->basecolor = ConstantColor::Create(colors.diffuse);
    }

    mat->normal_map = HasTexture(normal_map) ? textures.normal_map : ConstantColor::Create(0.5, 0.5, 1.0);

    if (HasTexture(metallic))
    {
        mat->metallic = textures.metallic;
    }
    else
    {
        if (IsBlack(colors.specular))
        {
            mat->metallic = ConstantColor::Create(0.0);
        }
        else
        {
            mat->metallic = ConstantColor::Create(1.0);
        }
    }

    if (HasTexture(roughness))
    {
        mat->roughness = textures.roughness;
    }
    else
    {
        mat->roughness = ConstantColor::Create(colors.specular);
    }

    if (HasTexture(emissive))
    {
        mat->emissive = textures.emissive;
    }
    else
    {
        mat->emissive = ConstantColor::Create(colors.emissive);
    }

    return mat;

#undef HasTexture
}

Ref<Microfacet> RandomMicrofacetMaterial()
{
    Ref<Microfacet> mat = CreateSharedRef<Microfacet>();

    Color basecolor = Vec3(Rand(0.0, 1.0), Rand(0.0, 1.0), Rand(0.0, 1.0)) * Float(0.7);
    mat->basecolor = ConstantColor::Create(basecolor);
    mat->roughness = ConstantColor::Create(Vec3(Rand(0.0, 1.0)));
    mat->metallic = ConstantColor::Create(Vec3(Rand() > 0.5 ? Float(1.0) : Float(0.0)));
    mat->emissive = ConstantColor::Create(basecolor * (Rand() < 0.08 ? Rand(0.0, Float(0.3)) : Float(0.0)));
    mat->normal_map = ConstantColor::Create(0.5, 0.5, 1.0);

    return mat;
}

Ref<Mesh> CreateRectXY(const Transform& tf, const Ref<Material> mat, const Point2& texCoord)
{
    Vec3 p0 = { -0.5, -0.5, 0.0 };
    Vec3 p1 = { 0.5, -0.5, 0.0 };
    Vec3 p2 = { 0.5, 0.5, 0.0 };
    Vec3 p3 = { -0.5, 0.5, 0.0 };

    Vertex v0{ p0, z_axis, x_axis, Point2(0.0, 0.0) };
    Vertex v1{ p1, z_axis, x_axis, Point2(texCoord.x, 0.0) };
    Vertex v2{ p2, z_axis, x_axis, Point2(texCoord.x, texCoord.y) };
    Vertex v3{ p3, z_axis, x_axis, Point2(0.0, texCoord.y) };

    auto vertices = std::vector<Vertex>{ v0, v1, v2, v3 };
    auto indices = std::vector<int32>{ 0, 1, 2, 0, 2, 3 };

    return CreateSharedRef<Mesh>(vertices, indices, tf, mat);
};

Ref<Mesh> CreateRectXZ(const Transform& tf, const Ref<Material> mat, const Point2& texCoord)
{
    Vec3 p0 = { -0.5, 0.0, 0.5 };
    Vec3 p1 = { 0.5, 0.0, 0.5 };
    Vec3 p2 = { 0.5, 0.0, -0.5 };
    Vec3 p3 = { -0.5, 0.0, -0.5 };

    Vertex v0{ p0, y_axis, x_axis, Point2(0.0, 0.0) };
    Vertex v1{ p1, y_axis, x_axis, Point2(texCoord.x, 0.0) };
    Vertex v2{ p2, y_axis, x_axis, Point2(texCoord.x, texCoord.y) };
    Vertex v3{ p3, y_axis, x_axis, Point2(0.0, texCoord.y) };

    auto vertices = std::vector<Vertex>{ v0, v1, v2, v3 };
    auto indices = std::vector<int32>{ 0, 1, 2, 0, 2, 3 };

    return CreateSharedRef<Mesh>(vertices, indices, tf, mat);
}

Ref<Mesh> CreateRectYZ(const Transform& tf, const Ref<Material> mat, const Point2& texCoord)
{
    Vec3 p0 = { 0.0, -0.5, 0.5 };
    Vec3 p1 = { 0.0, -0.5, -0.5 };
    Vec3 p2 = { 0.0, 0.5, -0.5 };
    Vec3 p3 = { 0.0, 0.5, 0.5 };

    Vertex v0{ p0, x_axis, -z_axis, Point2(0.0, 0.0) };
    Vertex v1{ p1, x_axis, -z_axis, Point2(texCoord.x, 0.0) };
    Vertex v2{ p2, x_axis, -z_axis, Point2(texCoord.x, texCoord.y) };
    Vertex v3{ p3, x_axis, -z_axis, Point2(0.0, texCoord.y) };

    auto vertices = std::vector<Vertex>{ v0, v1, v2, v3 };
    auto indices = std::vector<int32>{ 0, 1, 2, 0, 2, 3 };

    return CreateSharedRef<Mesh>(vertices, indices, tf, mat);
}

Ref<Mesh> CreateBox(const Transform& tf, const Ref<Material> mat, const Point2& texCoord)
{
    /*
          7--------6
         /|       /|
        3--------2 |
        | 4------|-5
        |/       |/
        0--------1
    */
    Vec3 p0 = { -0.5, -0.5, 0.5 };
    Vec3 p1 = { 0.5, -0.5, 0.5 };
    Vec3 p2 = { 0.5, 0.5, 0.5 };
    Vec3 p3 = { -0.5, 0.5, 0.5 };

    Vec3 p4 = { -0.5, -0.5, -0.5 };
    Vec3 p5 = { 0.5, -0.5, -0.5 };
    Vec3 p6 = { 0.5, 0.5, -0.5 };
    Vec3 p7 = { -0.5, 0.5, -0.5 };

    Vertex v00 = { p0, z_axis, x_axis, Point2(0.0, 0.0) };
    Vertex v01 = { p1, z_axis, x_axis, Point2(texCoord.x, 0.0) };
    Vertex v02 = { p2, z_axis, x_axis, Point2(texCoord.x, texCoord.y) };
    Vertex v03 = { p3, z_axis, x_axis, Point2(0.0, texCoord.y) };

    Vertex v04 = { p1, x_axis, -z_axis, Point2(0.0, 0.0) };
    Vertex v05 = { p5, x_axis, -z_axis, Point2(texCoord.x, 0.0) };
    Vertex v06 = { p6, x_axis, -z_axis, Point2(texCoord.x, texCoord.y) };
    Vertex v07 = { p2, x_axis, -z_axis, Point2(0.0, texCoord.y) };

    Vertex v08 = { p5, -z_axis, -x_axis, Point2(0.0, 0.0) };
    Vertex v09 = { p4, -z_axis, -x_axis, Point2(texCoord.x, 0.0) };
    Vertex v10 = { p7, -z_axis, -x_axis, Point2(texCoord.x, texCoord.y) };
    Vertex v11 = { p6, -z_axis, -x_axis, Point2(0.0, texCoord.y) };

    Vertex v12 = { p4, -x_axis, z_axis, Point2(0.0, 0.0) };
    Vertex v13 = { p0, -x_axis, z_axis, Point2(texCoord.x, 0.0) };
    Vertex v14 = { p3, -x_axis, z_axis, Point2(texCoord.x, texCoord.y) };
    Vertex v15 = { p7, -x_axis, z_axis, Point2(0.0, texCoord.y) };

    Vertex v16 = { p3, y_axis, x_axis, Point2(0.0, 0.0) };
    Vertex v17 = { p2, y_axis, x_axis, Point2(texCoord.x, 0.0) };
    Vertex v18 = { p6, y_axis, x_axis, Point2(texCoord.x, texCoord.y) };
    Vertex v19 = { p7, y_axis, x_axis, Point2(0.0, texCoord.y) };

    Vertex v20 = { p1, -y_axis, -x_axis, Point2(0.0, 0.0) };
    Vertex v21 = { p0, -y_axis, -x_axis, Point2(texCoord.x, 0.0) };
    Vertex v22 = { p4, -y_axis, -x_axis, Point2(texCoord.x, texCoord.y) };
    Vertex v23 = { p5, -y_axis, -x_axis, Point2(0.0, texCoord.y) };

    auto vertices = std::vector<Vertex>{ v00, v01, v02, v03, v04, v05, v06, v07, v08, v09, v10, v11,
                                         v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23 };

    // clang-format off
    auto indices = std::vector<int32>{
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };
    // clang-format on

    return CreateSharedRef<Mesh>(vertices, indices, tf, mat);
}

} // namespace spt
