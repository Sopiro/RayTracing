#pragma once

#include "common.h"
#include "material.h"
#include "mesh.h"
#include "microfacet.h"
#include "solid_color.h"
#include "transform.h"

namespace spt
{

inline bool IsNullish(f64 v)
{
    return std::isnan(v) || std::isinf(v);
}

inline bool IsNullish(const Vec2& v)
{
    return std::isnan(v.x) || std::isnan(v.y) || std::isinf(v.x) || std::isinf(v.y);
}

inline bool IsNullish(const Vec3& v)
{
    return std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z) || std::isinf(v.x) || std::isinf(v.y) || std::isinf(v.z);
}

inline bool IsNullish(const Vec4& v)
{
    return std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z) || std::isnan(v.w) || std::isinf(v.x) || std::isinf(v.y) ||
           std::isinf(v.z) || std::isinf(v.w);
}

inline bool IsBlack(Color color)
{
    return std::fabs(color.x) == 0.0 && std::fabs(color.y) == 0.0 && std::fabs(color.z) == 0.0;
}

#define checkNull(v)                                                                                                             \
    if (IsNullish(v))                                                                                                            \
    {                                                                                                                            \
        std::cout << #v;                                                                                                         \
        std::cout << " null" << std::endl;                                                                                       \
    }

struct MaterialTextures
{
    Ref<Texture> basecolor;
    Ref<Texture> normal;
    Ref<Texture> metallic;
    Ref<Texture> roughness;
    Ref<Texture> ao;
    Ref<Texture> emissive;
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

Ref<Mesh> CreateRectXY(const Transform& transform, const Ref<Material> material, const UV& texCoord = UV{ 1.0, 1.0 });
Ref<Mesh> CreateRectXZ(const Transform& transform, const Ref<Material> material, const UV& texCoord = UV{ 1.0, 1.0 });
Ref<Mesh> CreateRectYZ(const Transform& transform, const Ref<Material> material, const UV& texCoord = UV{ 1.0, 1.0 });
Ref<Mesh> CreateBox(const Transform& transform, const Ref<Material> material, const UV& texCoord = UV{ 1.0, 1.0 });

} // namespace spt
