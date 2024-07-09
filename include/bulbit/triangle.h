#pragma once

#include "mesh.h"
#include "ray.h"
#include "shape.h"

namespace bulbit
{

class Triangle : public Shape
{
public:
    Triangle(const Mesh* mesh, size_t tri_index);

    virtual AABB GetAABB() const override;
    virtual bool Intersect(Intersection* out_is, const Ray& ray, Float t_min, Float t_max) const override;
    virtual bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const override;

    virtual ShapeSample Sample(const Point2& u) const override;
    virtual ShapeSample Sample(const Point3& ref, const Point2& u) const override;

    virtual Float EvaluatePDF(const Ray& ray) const override;
    virtual Float PDF(const Intersection& hit_is, const Ray& hit_ray) const override;

private:
    friend class Scene;

    Vec3 GetNormal(Float u, Float v, Float w) const;
    Vec3 GetTangent(Float u, Float v, Float w) const;
    Point2 GetTexCoord(Float u, Float v, Float w) const;

    const Mesh* mesh;
    const int32* v;
};

inline Triangle::Triangle(const Mesh* mesh, size_t tri_index)
    : mesh{ mesh }
{
    v = &mesh->indices[tri_index * 3];
}

inline AABB Triangle::GetAABB() const
{
    const Vec3 aabb_offset{ epsilon * 10 };

    const Point3& p0 = mesh->positions[v[0]];
    const Point3& p1 = mesh->positions[v[1]];
    const Point3& p2 = mesh->positions[v[2]];

    Vec3 min = Min(Min(p0, p1), p2) - aabb_offset;
    Vec3 max = Max(Max(p0, p1), p2) + aabb_offset;

    return AABB(min, max);
}

inline Float Triangle::EvaluatePDF(const Ray& ray) const
{
    Intersection is;
    if (Intersect(&is, ray, Ray::epsilon, infinity) == false)
    {
        return 0;
    }

    return PDF(is, ray);
}

inline Float Triangle::PDF(const Intersection& hit_is, const Ray& hit_ray) const
{
    Float distance_squared = hit_is.t * hit_is.t * Length2(hit_ray.d);
    Float cosine = std::fabs(Dot(hit_ray.d, hit_is.normal) / Length(hit_ray.d));

    const Point3& p0 = mesh->positions[v[0]];
    const Point3& p1 = mesh->positions[v[1]];
    const Point3& p2 = mesh->positions[v[2]];

    Vec3 e1 = p1 - p0;
    Vec3 e2 = p2 - p0;

    Float area = 0.5f * Length(Cross(e1, e2));

    return distance_squared / (cosine * area);
}

inline Vec3 Triangle::GetNormal(Float tu, Float tv, Float tw) const
{
    const Vec3& n0 = mesh->normals[v[0]];
    const Vec3& n1 = mesh->normals[v[1]];
    const Vec3& n2 = mesh->normals[v[2]];

    return Normalize(tw * n0 + tu * n1 + tv * n2);
}

inline Vec3 Triangle::GetTangent(Float tu, Float tv, Float tw) const
{
    const Vec3& t0 = mesh->tangents[v[0]];
    const Vec3& t1 = mesh->tangents[v[1]];
    const Vec3& t2 = mesh->tangents[v[2]];

    return Normalize(tw * t0 + tu * t1 + tv * t2);
}

inline Point2 Triangle::GetTexCoord(Float tu, Float tv, Float tw) const
{
    const Point2& u0 = mesh->texCoords[v[0]];
    const Point2& u1 = mesh->texCoords[v[1]];
    const Point2& u2 = mesh->texCoords[v[2]];

    return tw * u0 + tu * u1 + tv * u2;
}

} // namespace bulbit