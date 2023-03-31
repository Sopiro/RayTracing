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

inline std::shared_ptr<PBRMaterial> RandomPBRMaterial()
{
    auto mat = std::make_shared<PBRMaterial>();

    auto basecolor = Vec3{ Prand(0.0, 1.0), Prand(0.0, 1.0), Prand(0.0, 1.0) } * 0.7;
    mat->basecolor_map = SolidColor::Create(basecolor);
    mat->normal_map = SolidColor::Create(0.5, 0.5, 1.0);
    mat->roughness_map = SolidColor::Create(Vec3{ Prand(0.0, 1.0) });
    mat->metallic_map = SolidColor::Create(Vec3{ Prand() > 0.8 ? 1.0 : 0.0 });
    mat->ao_map = SolidColor::Create(Vec3{ 0.0 });
    mat->emissive_map = SolidColor::Create(basecolor * (Prand() < 0.04 ? Prand(0.0, 0.2) : 0.0));

    return mat;
}

inline std::shared_ptr<Hittable> RectXY(const Transform& tf, const std::shared_ptr<Material> mat)
{
    Vec3 v0 = Mul(tf, Vec3{ -0.5, -0.5, 0.0 });
    Vec3 v1 = Mul(tf, Vec3{ 0.5, -0.5, 0.0 });
    Vec3 v2 = Mul(tf, Vec3{ 0.5, 0.5, 0.0 });
    Vec3 v3 = Mul(tf, Vec3{ -0.5, 0.5, 0.0 });

    auto t1 = std::make_shared<Triangle>(v0, v1, v2, mat);
    auto t2 = std::make_shared<Triangle>(v0, v2, v3, mat);

    t1->v0.texCoords.Set(0.0, 0.0);
    t1->v1.texCoords.Set(1.0, 0.0);
    t1->v2.texCoords.Set(1.0, 1.0);
    t2->v0.texCoords.Set(0.0, 0.0);
    t2->v1.texCoords.Set(1.0, 1.0);
    t2->v2.texCoords.Set(0.0, 1.0);

    auto rect = std::make_shared<HittableList>();
    rect->Add(t1);
    rect->Add(t2);

    return rect;
}

inline std::shared_ptr<Hittable> RectXZ(const Transform& tf, const std::shared_ptr<Material> mat)
{
    Vec3 v0 = Mul(tf, Vec3{ -0.5, 0.0, 0.5 });
    Vec3 v1 = Mul(tf, Vec3{ 0.5, 0.0, 0.5 });
    Vec3 v2 = Mul(tf, Vec3{ 0.5, 0.0, -0.5 });
    Vec3 v3 = Mul(tf, Vec3{ -0.5, 0.0, -0.5 });

    auto t1 = std::make_shared<Triangle>(v0, v1, v2, mat);
    auto t2 = std::make_shared<Triangle>(v0, v2, v3, mat);

    t1->v0.texCoords.Set(0.0, 0.0);
    t1->v1.texCoords.Set(1.0, 0.0);
    t1->v2.texCoords.Set(1.0, 1.0);
    t2->v0.texCoords.Set(0.0, 0.0);
    t2->v1.texCoords.Set(1.0, 1.0);
    t2->v2.texCoords.Set(0.0, 1.0);

    auto rect = std::make_shared<HittableList>();
    rect->Add(t1);
    rect->Add(t2);

    return rect;
}

inline std::shared_ptr<Hittable> RectYZ(const Transform& tf, const std::shared_ptr<Material> mat)
{
    Vec3 v0 = Mul(tf, Vec3{ 0.0, -0.5, 0.5 });
    Vec3 v1 = Mul(tf, Vec3{ 0.0, -0.5, -0.5 });
    Vec3 v2 = Mul(tf, Vec3{ 0.0, 0.5, -0.5 });
    Vec3 v3 = Mul(tf, Vec3{ 0.0, 0.5, 0.5 });

    auto t1 = std::make_shared<Triangle>(v0, v1, v2, mat);
    auto t2 = std::make_shared<Triangle>(v0, v2, v3, mat);

    t1->v0.texCoords.Set(0.0, 0.0);
    t1->v1.texCoords.Set(1.0, 0.0);
    t1->v2.texCoords.Set(1.0, 1.0);
    t2->v0.texCoords.Set(0.0, 0.0);
    t2->v1.texCoords.Set(1.0, 1.0);
    t2->v2.texCoords.Set(0.0, 1.0);

    auto rect = std::make_shared<HittableList>();
    rect->Add(t1);
    rect->Add(t2);

    return rect;
}

inline std::shared_ptr<Hittable> Box(const Transform& tf, const std::shared_ptr<Material> mat)
{
    Vec3 v0 = Mul(tf, Vec3{ -0.5, -0.5, 0.5 });
    Vec3 v1 = Mul(tf, Vec3{ 0.5, -0.5, 0.5 });
    Vec3 v2 = Mul(tf, Vec3{ 0.5, 0.5, 0.5 });
    Vec3 v3 = Mul(tf, Vec3{ -0.5, 0.5, 0.5 });

    Vec3 v4 = Mul(tf, Vec3{ -0.5, -0.5, -0.5 });
    Vec3 v5 = Mul(tf, Vec3{ 0.5, -0.5, -0.5 });
    Vec3 v6 = Mul(tf, Vec3{ 0.5, 0.5, -0.5 });
    Vec3 v7 = Mul(tf, Vec3{ -0.5, 0.5, -0.5 });

    auto box = std::make_shared<HittableList>();

    // front
    box->Add(std::make_shared<Triangle>(v0, v1, v2, mat));
    box->Add(std::make_shared<Triangle>(v0, v2, v3, mat));

    // right
    box->Add(std::make_shared<Triangle>(v1, v5, v6, mat));
    box->Add(std::make_shared<Triangle>(v1, v6, v2, mat));

    // back
    box->Add(std::make_shared<Triangle>(v5, v4, v7, mat));
    box->Add(std::make_shared<Triangle>(v5, v7, v6, mat));

    // left
    box->Add(std::make_shared<Triangle>(v4, v0, v3, mat));
    box->Add(std::make_shared<Triangle>(v4, v3, v7, mat));

    // top
    box->Add(std::make_shared<Triangle>(v3, v2, v6, mat));
    box->Add(std::make_shared<Triangle>(v3, v6, v7, mat));

    // bottom
    box->Add(std::make_shared<Triangle>(v1, v0, v4, mat));
    box->Add(std::make_shared<Triangle>(v1, v4, v5, mat));

    return box;
}

} // namespace spt
