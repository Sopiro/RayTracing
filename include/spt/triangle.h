#pragma once

#include "common.h"
#include "mesh.h"
#include "primitive.h"
#include "ray.h"

namespace spt
{

class Triangle : public Primitive
{
public:
    Triangle() = default;
    Triangle(const Ref<Mesh> mesh, size_t tri_index);

    virtual bool Intersect(Intersection* out_is, const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual bool IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual void GetAABB(AABB* out_aabb) const override;

    virtual Point3 Sample() const override;
    virtual Point3 Sample(const Point3& ref) const override;
    virtual f64 EvaluatePDF(const Ray& ray) const override;
    virtual f64 PDFValue(const Intersection& hit_is, const Ray& hit_ray) const override;
    virtual const Material* GetMaterial() const override;

private:
    friend class Scene;

    Vec3 GetShadingNormal(f64 u, f64 v, f64 w) const;
    Vec3 GetShadingTangent(f64 u, f64 v, f64 w) const;
    UV GetTexCoord(f64 u, f64 v, f64 w) const;

    const Ref<Mesh> mesh;
    const i32* v;

    bool two_sided = true;
};

inline Triangle::Triangle(const Ref<Mesh> _mesh, size_t tri_index)
    : mesh{ _mesh }
{
    v = &mesh->indices[tri_index * 3];
}

inline void Triangle::GetAABB(AABB* out_aabb) const
{
    const Vec3 aabb_offset{ epsilon * 10.0 };

    const Vec3& p0 = mesh->vertices[v[0]].position;
    const Vec3& p1 = mesh->vertices[v[1]].position;
    const Vec3& p2 = mesh->vertices[v[2]].position;

    out_aabb->min = Min(Min(p0, p1), p2) - aabb_offset;
    out_aabb->max = Max(Max(p0, p1), p2) + aabb_offset;
}

inline Point3 Triangle::Sample() const
{
    const Vec3& p0 = mesh->vertices[v[0]].position;
    const Vec3& p1 = mesh->vertices[v[1]].position;
    const Vec3& p2 = mesh->vertices[v[2]].position;

    Vec3 e1 = p1 - p0;
    Vec3 e2 = p2 - p0;

#if 1
    f64 u = Rand(0.0, 1.0);
    f64 v = Rand(0.0, 1.0);

    if (u + v > 1.0)
    {
        u = 1.0 - u;
        v = 1.0 - v;
    }

    return p0 + e1 * u + e2 * v;
#else
    f64 u1 = Rand(0.0, 1.0);
    f64 u2 = Rand(0.0, 1.0);

    f64 s = sqrt(u1);
    f64 u = 1.0 - s;
    f64 v = u2 * s;

    return v0.position + e1 * u + e2 * v;
#endif
}

inline Point3 Triangle::Sample(const Point3& ref) const
{
    return Sample();
}

inline f64 Triangle::EvaluatePDF(const Ray& ray) const
{
    Intersection is;
    if (Intersect(&is, ray, ray_offset, infinity) == false)
    {
        return 0.0;
    }

    return PDFValue(is, ray);
}

inline f64 Triangle::PDFValue(const Intersection& hit_is, const Ray& hit_ray) const
{
    f64 distance_squared = hit_is.t * hit_is.t * hit_ray.dir.Length2();
    f64 cosine = fabs(Dot(hit_ray.dir, hit_is.normal) / hit_ray.dir.Length());

    const Vec3& p0 = mesh->vertices[v[0]].position;
    const Vec3& p1 = mesh->vertices[v[1]].position;
    const Vec3& p2 = mesh->vertices[v[2]].position;

    Vec3 e1 = p1 - p0;
    Vec3 e2 = p2 - p0;

    f64 area = 0.5 * Cross(e1, e2).Length();

    return distance_squared / (cosine * area);
}

inline const Material* Triangle::GetMaterial() const
{
    return mesh->material.get();
}

inline Vec3 Triangle::GetShadingNormal(f64 _u, f64 _v, f64 _w) const
{
    const Vec3& n0 = mesh->vertices[v[0]].normal;
    const Vec3& n1 = mesh->vertices[v[1]].normal;
    const Vec3& n2 = mesh->vertices[v[2]].normal;

    return (_w * n0 + _u * n1 + _v * n2).Normalized();
}

inline Vec3 Triangle::GetShadingTangent(f64 _u, f64 _v, f64 _w) const
{
    const Vec3& t0 = mesh->vertices[v[0]].normal;
    const Vec3& t1 = mesh->vertices[v[1]].normal;
    const Vec3& t2 = mesh->vertices[v[2]].normal;

    return (_w * t0 + _u * t1 + _v * t2).Normalized();
}

inline UV Triangle::GetTexCoord(f64 _u, f64 _v, f64 _w) const
{
    const Vec2& u0 = mesh->vertices[v[0]].texCoord;
    const Vec2& u1 = mesh->vertices[v[1]].texCoord;
    const Vec2& u2 = mesh->vertices[v[2]].texCoord;

    return _w * u0 + _u * u1 + _v * u2;
}
} // namespace spt