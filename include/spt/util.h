#pragma once

#include "constant_color.h"
#include "mesh.h"
#include "transform.h"

namespace spt
{

struct MaterialTextures
{
    Ref<Texture> basecolor;
    Ref<Texture> metallic;
    Ref<Texture> roughness;
    Ref<Texture> emissive;
    Ref<Texture> normal_map;
};

struct MaterialColors
{
    Color diffuse;
    Color specular;
    Color emissive;
};

// Create microfacet material with given textures and colors
Ref<Material> CreateMaterial(const MaterialTextures& textures, const MaterialColors& colors);
Ref<Microfacet> RandomMicrofacetMaterial();

Ref<Mesh> CreateRectXY(const Transform& transform,
                       const Ref<Material> material,
                       const Point2& texCoord = Point2(Float(1.0), Float(1.0)));
Ref<Mesh> CreateRectXZ(const Transform& transform,
                       const Ref<Material> material,
                       const Point2& texCoord = Point2(Float(1.0), Float(1.0)));
Ref<Mesh> CreateRectYZ(const Transform& transform,
                       const Ref<Material> material,
                       const Point2& texCoord = Point2(Float(1.0), Float(1.0)));
Ref<Mesh> CreateBox(const Transform& transform,
                    const Ref<Material> material,
                    const Point2& texCoord = Point2(Float(1.0), Float(1.0)));

inline bool IsNullish(Float v)
{
    return std::isnan(v) || std::isinf(v);
}

template <typename T>
inline bool IsNullish(const T& v)
{
    return v.IsNullish();
}

inline bool IsBlack(Color color)
{
    return std::fabs(color.x) == Float(0.0) && std::fabs(color.y) == Float(0.0) && std::fabs(color.z) == Float(0.0);
}

// Luminance of linear color
// https://en.wikipedia.org/wiki/Luma_(video)
inline Float Luma(Vec3 srgb)
{
    return Dot(srgb, Vec3(Float(0.2126), Float(0.7152), Float(0.0722)));
}

#define checkNull(v)                                                                                                             \
    if (IsNullish(v))                                                                                                            \
    {                                                                                                                            \
        std::cout << #v;                                                                                                         \
        std::cout << " null" << std::endl;                                                                                       \
    }

} // namespace spt
