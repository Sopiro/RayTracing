#include "spt/util.h"
#include "spt/mesh.h"
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

Ref<Mesh> CreateRectXY(const Transform& tf, const Ref<Material>& mat, const UV& texCoord)
{
    Vec3 v0 = Mul(tf, Vec3{ -0.5, -0.5, 0.0 });
    Vec3 v1 = Mul(tf, Vec3{ 0.5, -0.5, 0.0 });
    Vec3 v2 = Mul(tf, Vec3{ 0.5, 0.5, 0.0 });
    Vec3 v3 = Mul(tf, Vec3{ -0.5, 0.5, 0.0 });

    auto t1 = Triangle{ v0, v1, v2, mat };
    auto t2 = Triangle{ v0, v2, v3, mat };

    t1.v0.texCoord.Set(0.0, 0.0);
    t1.v1.texCoord.Set(texCoord.x, 0.0);
    t1.v2.texCoord.Set(texCoord.x, texCoord.y);
    t2.v0.texCoord.Set(0.0, 0.0);
    t2.v1.texCoord.Set(texCoord.x, texCoord.y);
    t2.v2.texCoord.Set(0.0, texCoord.y);

    return CreateSharedRef<Mesh>(std::vector<Triangle>{ t1, t2 }, mat);
};

Ref<Mesh> CreateRectXZ(const Transform& tf, const Ref<Material>& mat, const UV& texCoord)
{
    Vec3 v0 = Mul(tf, Vec3{ -0.5, 0.0, 0.5 });
    Vec3 v1 = Mul(tf, Vec3{ 0.5, 0.0, 0.5 });
    Vec3 v2 = Mul(tf, Vec3{ 0.5, 0.0, -0.5 });
    Vec3 v3 = Mul(tf, Vec3{ -0.5, 0.0, -0.5 });

    auto t1 = Triangle{ v0, v1, v2, mat };
    auto t2 = Triangle{ v0, v2, v3, mat };

    t1.v0.texCoord.Set(0.0, 0.0);
    t1.v1.texCoord.Set(texCoord.x, 0.0);
    t1.v2.texCoord.Set(texCoord.x, texCoord.y);
    t2.v0.texCoord.Set(0.0, 0.0);
    t2.v1.texCoord.Set(texCoord.x, texCoord.y);
    t2.v2.texCoord.Set(0.0, texCoord.y);

    return CreateSharedRef<Mesh>(std::vector<Triangle>{ t1, t2 }, mat);
}

Ref<Mesh> CreateRectYZ(const Transform& tf, const Ref<Material>& mat, const UV& texCoord)
{
    Vec3 v0 = Mul(tf, Vec3{ 0.0, -0.5, 0.5 });
    Vec3 v1 = Mul(tf, Vec3{ 0.0, -0.5, -0.5 });
    Vec3 v2 = Mul(tf, Vec3{ 0.0, 0.5, -0.5 });
    Vec3 v3 = Mul(tf, Vec3{ 0.0, 0.5, 0.5 });

    auto t1 = Triangle{ v0, v1, v2, mat };
    auto t2 = Triangle{ v0, v2, v3, mat };

    t1.v0.texCoord.Set(0.0, 0.0);
    t1.v1.texCoord.Set(texCoord.x, 0.0);
    t1.v2.texCoord.Set(texCoord.x, texCoord.y);
    t2.v0.texCoord.Set(0.0, 0.0);
    t2.v1.texCoord.Set(texCoord.x, texCoord.y);
    t2.v2.texCoord.Set(0.0, texCoord.y);

    return CreateSharedRef<Mesh>(std::vector<Triangle>{ t1, t2 }, mat);
}

Ref<Mesh> CreateBox(const Transform& tf, const Ref<Material>& mat)
{
    Vec3 v0 = Mul(tf, Vec3{ -0.5, -0.5, 0.5 });
    Vec3 v1 = Mul(tf, Vec3{ 0.5, -0.5, 0.5 });
    Vec3 v2 = Mul(tf, Vec3{ 0.5, 0.5, 0.5 });
    Vec3 v3 = Mul(tf, Vec3{ -0.5, 0.5, 0.5 });

    Vec3 v4 = Mul(tf, Vec3{ -0.5, -0.5, -0.5 });
    Vec3 v5 = Mul(tf, Vec3{ 0.5, -0.5, -0.5 });
    Vec3 v6 = Mul(tf, Vec3{ 0.5, 0.5, -0.5 });
    Vec3 v7 = Mul(tf, Vec3{ -0.5, 0.5, -0.5 });

    // front
    auto t1 = Triangle(v0, v1, v2, mat);
    t1.v0.texCoord.Set(0.0, 0.0);
    t1.v1.texCoord.Set(1.0, 0.0);
    t1.v2.texCoord.Set(1.0, 1.0);

    auto t2 = Triangle(v0, v2, v3, mat);
    t2.v0.texCoord.Set(0.0, 0.0);
    t2.v1.texCoord.Set(1.0, 1.0);
    t2.v2.texCoord.Set(0.0, 1.0);

    // right
    auto t3 = Triangle(v1, v5, v6, mat);
    t3.v0.texCoord.Set(0.0, 0.0);
    t3.v1.texCoord.Set(1.0, 0.0);
    t3.v2.texCoord.Set(1.0, 1.0);

    auto t4 = Triangle(v1, v6, v2, mat);
    t4.v0.texCoord.Set(0.0, 0.0);
    t4.v1.texCoord.Set(1.0, 1.0);
    t4.v2.texCoord.Set(0.0, 1.0);

    // back
    auto t5 = Triangle(v5, v4, v7, mat);
    t5.v0.texCoord.Set(0.0, 0.0);
    t5.v1.texCoord.Set(1.0, 0.0);
    t5.v2.texCoord.Set(1.0, 1.0);

    auto t6 = Triangle(v5, v7, v6, mat);
    t6.v0.texCoord.Set(0.0, 0.0);
    t6.v1.texCoord.Set(1.0, 1.0);
    t6.v2.texCoord.Set(0.0, 1.0);

    // left
    auto t7 = Triangle(v4, v0, v3, mat);
    t7.v0.texCoord.Set(0.0, 0.0);
    t7.v1.texCoord.Set(1.0, 0.0);
    t7.v2.texCoord.Set(1.0, 1.0);

    auto t8 = Triangle(v4, v3, v7, mat);
    t8.v0.texCoord.Set(0.0, 0.0);
    t8.v1.texCoord.Set(1.0, 1.0);
    t8.v2.texCoord.Set(0.0, 1.0);

    // top
    auto t9 = Triangle(v3, v2, v6, mat);
    t9.v0.texCoord.Set(0.0, 0.0);
    t9.v1.texCoord.Set(1.0, 0.0);
    t9.v2.texCoord.Set(1.0, 1.0);

    auto t10 = Triangle(v3, v6, v7, mat);
    t10.v0.texCoord.Set(0.0, 0.0);
    t10.v1.texCoord.Set(1.0, 1.0);
    t10.v2.texCoord.Set(0.0, 1.0);

    // bottom
    auto t11 = Triangle(v1, v0, v4, mat);
    t11.v0.texCoord.Set(0.0, 0.0);
    t11.v1.texCoord.Set(1.0, 0.0);
    t11.v2.texCoord.Set(1.0, 1.0);

    auto t12 = Triangle(v1, v4, v5, mat);
    t12.v0.texCoord.Set(0.0, 0.0);
    t12.v1.texCoord.Set(1.0, 1.0);
    t12.v2.texCoord.Set(0.0, 1.0);

    return CreateSharedRef<Mesh>(std::vector<Triangle>{ t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12 }, mat);
}

} // namespace spt
