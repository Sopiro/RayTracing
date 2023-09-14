#pragma once

#include "common.h"
#include "material.h"
#include "mesh.h"
#include "microfacet.h"
#include "solid_color.h"
#include "transform.h"

namespace spt
{

inline bool is_nullish(f64 v)
{
    return isnan(v) || isinf(v);
}

inline bool is_nullish(const Vec2& v)
{
    return isnan(v.x) || isnan(v.y) || isinf(v.x) || isinf(v.y);
}

inline bool is_nullish(const Vec3& v)
{
    return isnan(v.x) || isnan(v.y) || isnan(v.z) || isinf(v.x) || isinf(v.y) || isinf(v.z);
}

inline bool is_nullish(const Vec4& v)
{
    return isnan(v.x) || isnan(v.y) || isnan(v.z) || isnan(v.w) || isinf(v.x) || isinf(v.y) || isinf(v.z) || isinf(v.w);
}

Ref<Microfacet> RandomPBRMaterial();
Ref<Mesh> CreateRectXY(const Transform& tf, const Ref<Material> mat, const UV& texCoord = UV{ 1.0, 1.0 });
Ref<Mesh> CreateRectXZ(const Transform& tf, const Ref<Material> mat, const UV& texCoord = UV{ 1.0, 1.0 });
Ref<Mesh> CreateRectYZ(const Transform& tf, const Ref<Material> mat, const UV& texCoord = UV{ 1.0, 1.0 });
Ref<Mesh> CreateBox(const Transform& tf, const Ref<Material> mat);

} // namespace spt
