#pragma once

#include "common.h"
#include "material.h"
#include "pbr_material.h"
#include "solid_color.h"
#include "transform.h"

namespace spt
{

inline bool is_nullish(const Vec3& v)
{
    return (isnan(v.x) || isnan(v.y) || isnan(v.z) || isinf(v.x) || isinf(v.y) || isinf(v.z));
}

inline Ref<PBRMaterial> RandomPBRMaterial()
{
    Ref<PBRMaterial> mat = CreateSharedRef<PBRMaterial>();

    Color basecolor = Vec3{ Rand(0.0, 1.0), Rand(0.0, 1.0), Rand(0.0, 1.0) } * 0.7;
    mat->basecolor_map = SolidColor::Create(basecolor);
    mat->normal_map = SolidColor::Create(0.5, 0.5, 1.0);
    mat->roughness_map = SolidColor::Create(Vec3{ Rand(0.0, 1.0) });
    mat->metallic_map = SolidColor::Create(Vec3{ Rand() > 0.5 ? 1.0 : 0.0 });
    mat->ao_map = SolidColor::Create(Vec3{ 1.0 });
    mat->emissive_map = SolidColor::Create(basecolor * (Rand() < 0.04 ? Rand(0.0, 0.2) : 0.0));

    return mat;
}

inline Ref<Hittable> RectXY(const Transform& tf, const Ref<Material>& mat, const UV& texCoords = UV{ 1.0, 1.0 })
{
    Vec3 v0 = Mul(tf, Vec3{ -0.5, -0.5, 0.0 });
    Vec3 v1 = Mul(tf, Vec3{ 0.5, -0.5, 0.0 });
    Vec3 v2 = Mul(tf, Vec3{ 0.5, 0.5, 0.0 });
    Vec3 v3 = Mul(tf, Vec3{ -0.5, 0.5, 0.0 });

    auto t1 = CreateSharedRef<Triangle>(v0, v1, v2, mat);
    auto t2 = CreateSharedRef<Triangle>(v0, v2, v3, mat);

    t1->v0.texCoords.Set(0.0, 0.0);
    t1->v1.texCoords.Set(texCoords.x, 0.0);
    t1->v2.texCoords.Set(texCoords.x, texCoords.y);
    t2->v0.texCoords.Set(0.0, 0.0);
    t2->v1.texCoords.Set(texCoords.x, texCoords.y);
    t2->v2.texCoords.Set(0.0, texCoords.y);

    auto rect = CreateSharedRef<HittableList>();
    rect->Add(t1);
    rect->Add(t2);

    return rect;
}

inline Ref<Hittable> RectXZ(const Transform& tf, const Ref<Material>& mat, const UV& texCoords = UV{ 1.0, 1.0 })
{
    Vec3 v0 = Mul(tf, Vec3{ -0.5, 0.0, 0.5 });
    Vec3 v1 = Mul(tf, Vec3{ 0.5, 0.0, 0.5 });
    Vec3 v2 = Mul(tf, Vec3{ 0.5, 0.0, -0.5 });
    Vec3 v3 = Mul(tf, Vec3{ -0.5, 0.0, -0.5 });

    auto t1 = CreateSharedRef<Triangle>(v0, v1, v2, mat);
    auto t2 = CreateSharedRef<Triangle>(v0, v2, v3, mat);

    t1->v0.texCoords.Set(0.0, 0.0);
    t1->v1.texCoords.Set(texCoords.x, 0.0);
    t1->v2.texCoords.Set(texCoords.x, texCoords.y);
    t2->v0.texCoords.Set(0.0, 0.0);
    t2->v1.texCoords.Set(texCoords.x, texCoords.y);
    t2->v2.texCoords.Set(0.0, texCoords.y);

    auto rect = CreateSharedRef<HittableList>();
    rect->Add(t1);
    rect->Add(t2);

    return rect;
}

inline Ref<Hittable> RectYZ(const Transform& tf, const Ref<Material>& mat, const UV& texCoords = UV{ 1.0, 1.0 })
{
    Vec3 v0 = Mul(tf, Vec3{ 0.0, -0.5, 0.5 });
    Vec3 v1 = Mul(tf, Vec3{ 0.0, -0.5, -0.5 });
    Vec3 v2 = Mul(tf, Vec3{ 0.0, 0.5, -0.5 });
    Vec3 v3 = Mul(tf, Vec3{ 0.0, 0.5, 0.5 });

    auto t1 = CreateSharedRef<Triangle>(v0, v1, v2, mat);
    auto t2 = CreateSharedRef<Triangle>(v0, v2, v3, mat);

    t1->v0.texCoords.Set(0.0, 0.0);
    t1->v1.texCoords.Set(texCoords.x, 0.0);
    t1->v2.texCoords.Set(texCoords.x, texCoords.y);
    t2->v0.texCoords.Set(0.0, 0.0);
    t2->v1.texCoords.Set(texCoords.x, texCoords.y);
    t2->v2.texCoords.Set(0.0, texCoords.y);

    auto rect = CreateSharedRef<HittableList>();
    rect->Add(t1);
    rect->Add(t2);

    return rect;
}

inline Ref<Hittable> Box(const Transform& tf, const Ref<Material>& mat)
{
    Vec3 v0 = Mul(tf, Vec3{ -0.5, -0.5, 0.5 });
    Vec3 v1 = Mul(tf, Vec3{ 0.5, -0.5, 0.5 });
    Vec3 v2 = Mul(tf, Vec3{ 0.5, 0.5, 0.5 });
    Vec3 v3 = Mul(tf, Vec3{ -0.5, 0.5, 0.5 });

    Vec3 v4 = Mul(tf, Vec3{ -0.5, -0.5, -0.5 });
    Vec3 v5 = Mul(tf, Vec3{ 0.5, -0.5, -0.5 });
    Vec3 v6 = Mul(tf, Vec3{ 0.5, 0.5, -0.5 });
    Vec3 v7 = Mul(tf, Vec3{ -0.5, 0.5, -0.5 });

    auto box = CreateSharedRef<HittableList>();

    // front
    box->Add(CreateSharedRef<Triangle>(v0, v1, v2, mat));
    box->Add(CreateSharedRef<Triangle>(v0, v2, v3, mat));

    // right
    box->Add(CreateSharedRef<Triangle>(v1, v5, v6, mat));
    box->Add(CreateSharedRef<Triangle>(v1, v6, v2, mat));

    // back
    box->Add(CreateSharedRef<Triangle>(v5, v4, v7, mat));
    box->Add(CreateSharedRef<Triangle>(v5, v7, v6, mat));

    // left
    box->Add(CreateSharedRef<Triangle>(v4, v0, v3, mat));
    box->Add(CreateSharedRef<Triangle>(v4, v3, v7, mat));

    // top
    box->Add(CreateSharedRef<Triangle>(v3, v2, v6, mat));
    box->Add(CreateSharedRef<Triangle>(v3, v6, v7, mat));

    // bottom
    box->Add(CreateSharedRef<Triangle>(v1, v0, v4, mat));
    box->Add(CreateSharedRef<Triangle>(v1, v4, v5, mat));

    return box;
}

} // namespace spt
