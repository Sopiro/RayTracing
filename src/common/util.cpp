#include "spt/util.h"
#include "spt/intersectable_list.h"
#include "spt/triangle.h"

namespace spt
{

Ref<Microfacet> RandomPBRMaterial()
{
    Ref<Microfacet> mat = CreateSharedRef<Microfacet>();

    Color basecolor = Vec3{ Rand(0.0, 1.0), Rand(0.0, 1.0), Rand(0.0, 1.0) } * 0.7;
    mat->basecolor_map = SolidColor::Create(basecolor);
    mat->normal_map = SolidColor::Create(0.5, 0.5, 1.0);
    mat->roughness_map = SolidColor::Create(Vec3{ Rand(0.0, 1.0) });
    mat->metallic_map = SolidColor::Create(Vec3{ Rand() > 0.5 ? 1.0 : 0.0 });
    mat->ao_map = SolidColor::Create(Vec3{ 1.0 });
    mat->emissive_map = SolidColor::Create(basecolor * (Rand() < 0.04 ? Rand(0.0, 0.2) : 0.0));

    return mat;
}

Ref<IntersectableList> RectXY(const Transform& tf, const Ref<Material>& mat, const UV& texCoord)
{
    Vec3 v0 = Mul(tf, Vec3{ -0.5, -0.5, 0.0 });
    Vec3 v1 = Mul(tf, Vec3{ 0.5, -0.5, 0.0 });
    Vec3 v2 = Mul(tf, Vec3{ 0.5, 0.5, 0.0 });
    Vec3 v3 = Mul(tf, Vec3{ -0.5, 0.5, 0.0 });

    auto t1 = CreateSharedRef<Triangle>(v0, v1, v2, mat);
    auto t2 = CreateSharedRef<Triangle>(v0, v2, v3, mat);

    t1->v0.texCoord.Set(0.0, 0.0);
    t1->v1.texCoord.Set(texCoord.x, 0.0);
    t1->v2.texCoord.Set(texCoord.x, texCoord.y);
    t2->v0.texCoord.Set(0.0, 0.0);
    t2->v1.texCoord.Set(texCoord.x, texCoord.y);
    t2->v2.texCoord.Set(0.0, texCoord.y);

    auto rect = CreateSharedRef<IntersectableList>();
    rect->Add(t1);
    rect->Add(t2);

    return rect;
}

Ref<IntersectableList> RectXZ(const Transform& tf, const Ref<Material>& mat, const UV& texCoord)
{
    Vec3 v0 = Mul(tf, Vec3{ -0.5, 0.0, 0.5 });
    Vec3 v1 = Mul(tf, Vec3{ 0.5, 0.0, 0.5 });
    Vec3 v2 = Mul(tf, Vec3{ 0.5, 0.0, -0.5 });
    Vec3 v3 = Mul(tf, Vec3{ -0.5, 0.0, -0.5 });

    auto t1 = CreateSharedRef<Triangle>(v0, v1, v2, mat);
    auto t2 = CreateSharedRef<Triangle>(v0, v2, v3, mat);

    t1->v0.texCoord.Set(0.0, 0.0);
    t1->v1.texCoord.Set(texCoord.x, 0.0);
    t1->v2.texCoord.Set(texCoord.x, texCoord.y);
    t2->v0.texCoord.Set(0.0, 0.0);
    t2->v1.texCoord.Set(texCoord.x, texCoord.y);
    t2->v2.texCoord.Set(0.0, texCoord.y);

    auto rect = CreateSharedRef<IntersectableList>();
    rect->Add(t1);
    rect->Add(t2);

    return rect;
}

Ref<IntersectableList> RectYZ(const Transform& tf, const Ref<Material>& mat, const UV& texCoord)
{
    Vec3 v0 = Mul(tf, Vec3{ 0.0, -0.5, 0.5 });
    Vec3 v1 = Mul(tf, Vec3{ 0.0, -0.5, -0.5 });
    Vec3 v2 = Mul(tf, Vec3{ 0.0, 0.5, -0.5 });
    Vec3 v3 = Mul(tf, Vec3{ 0.0, 0.5, 0.5 });

    auto t1 = CreateSharedRef<Triangle>(v0, v1, v2, mat);
    auto t2 = CreateSharedRef<Triangle>(v0, v2, v3, mat);

    t1->v0.texCoord.Set(0.0, 0.0);
    t1->v1.texCoord.Set(texCoord.x, 0.0);
    t1->v2.texCoord.Set(texCoord.x, texCoord.y);
    t2->v0.texCoord.Set(0.0, 0.0);
    t2->v1.texCoord.Set(texCoord.x, texCoord.y);
    t2->v2.texCoord.Set(0.0, texCoord.y);

    auto rect = CreateSharedRef<IntersectableList>();
    rect->Add(t1);
    rect->Add(t2);

    return rect;
}

Ref<IntersectableList> Box(const Transform& tf, const Ref<Material>& mat)
{
    Vec3 v0 = Mul(tf, Vec3{ -0.5, -0.5, 0.5 });
    Vec3 v1 = Mul(tf, Vec3{ 0.5, -0.5, 0.5 });
    Vec3 v2 = Mul(tf, Vec3{ 0.5, 0.5, 0.5 });
    Vec3 v3 = Mul(tf, Vec3{ -0.5, 0.5, 0.5 });

    Vec3 v4 = Mul(tf, Vec3{ -0.5, -0.5, -0.5 });
    Vec3 v5 = Mul(tf, Vec3{ 0.5, -0.5, -0.5 });
    Vec3 v6 = Mul(tf, Vec3{ 0.5, 0.5, -0.5 });
    Vec3 v7 = Mul(tf, Vec3{ -0.5, 0.5, -0.5 });

    auto box = CreateSharedRef<IntersectableList>();

    // front
    {
        auto t = CreateSharedRef<Triangle>(v0, v1, v2, mat);
        t->v0.texCoord.Set(0.0, 0.0);
        t->v1.texCoord.Set(1.0, 0.0);
        t->v2.texCoord.Set(1.0, 1.0);
        box->Add(t);

        t = CreateSharedRef<Triangle>(v0, v2, v3, mat);
        t->v0.texCoord.Set(0.0, 0.0);
        t->v1.texCoord.Set(1.0, 1.0);
        t->v2.texCoord.Set(0.0, 1.0);
        box->Add(t);
    }

    // right
    {
        auto t = CreateSharedRef<Triangle>(v1, v5, v6, mat);
        t->v0.texCoord.Set(0.0, 0.0);
        t->v1.texCoord.Set(1.0, 0.0);
        t->v2.texCoord.Set(1.0, 1.0);
        box->Add(t);

        t = CreateSharedRef<Triangle>(v1, v6, v2, mat);
        t->v0.texCoord.Set(0.0, 0.0);
        t->v1.texCoord.Set(1.0, 1.0);
        t->v2.texCoord.Set(0.0, 1.0);
        box->Add(t);
    }

    // back
    {
        auto t = CreateSharedRef<Triangle>(v5, v4, v7, mat);
        t->v0.texCoord.Set(0.0, 0.0);
        t->v1.texCoord.Set(1.0, 0.0);
        t->v2.texCoord.Set(1.0, 1.0);
        box->Add(t);

        t = CreateSharedRef<Triangle>(v5, v7, v6, mat);
        t->v0.texCoord.Set(0.0, 0.0);
        t->v1.texCoord.Set(1.0, 1.0);
        t->v2.texCoord.Set(0.0, 1.0);
        box->Add(t);
    }

    // left
    {
        auto t = CreateSharedRef<Triangle>(v4, v0, v3, mat);
        t->v0.texCoord.Set(0.0, 0.0);
        t->v1.texCoord.Set(1.0, 0.0);
        t->v2.texCoord.Set(1.0, 1.0);
        box->Add(t);

        t = CreateSharedRef<Triangle>(v4, v3, v7, mat);
        t->v0.texCoord.Set(0.0, 0.0);
        t->v1.texCoord.Set(1.0, 1.0);
        t->v2.texCoord.Set(0.0, 1.0);
        box->Add(t);
    }

    // top
    {
        auto t = CreateSharedRef<Triangle>(v3, v2, v6, mat);
        t->v0.texCoord.Set(0.0, 0.0);
        t->v1.texCoord.Set(1.0, 0.0);
        t->v2.texCoord.Set(1.0, 1.0);
        box->Add(t);

        t = CreateSharedRef<Triangle>(v3, v6, v7, mat);
        t->v0.texCoord.Set(0.0, 0.0);
        t->v1.texCoord.Set(1.0, 1.0);
        t->v2.texCoord.Set(0.0, 1.0);
        box->Add(t);
    }

    // bottom
    {
        auto t = CreateSharedRef<Triangle>(v1, v0, v4, mat);
        t->v0.texCoord.Set(0.0, 0.0);
        t->v1.texCoord.Set(1.0, 0.0);
        t->v2.texCoord.Set(1.0, 1.0);
        box->Add(t);

        t = CreateSharedRef<Triangle>(v1, v4, v5, mat);
        t->v0.texCoord.Set(0.0, 0.0);
        t->v1.texCoord.Set(1.0, 1.0);
        t->v2.texCoord.Set(0.0, 1.0);
        box->Add(t);
    }

    return box;
}

} // namespace spt
