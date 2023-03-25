#pragma once

#include "common.h"
#include "hittable.h"
#include "mesh.h"
#include "ray.h"

namespace spt
{

class Triangle : public Hittable
{
public:
    Triangle() = default;
    Triangle(const Vertex& vertex0,
             const Vertex& vertex1,
             const Vertex& vertex2,
             std::shared_ptr<Material> material,
             bool double_sided = true,
             bool reset_normal = false);

    Vec3 GetNormal(double u, double v, double w) const;
    Vec2 GetTexCoord(double u, double v, double w) const;

    virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;
    virtual double EvaluatePDF(const Ray& ray) const override;
    virtual double PDFValue(const Ray& hit_ray, const HitRecord& hit_rec) const override;
    virtual Vec3 GetRandomDirection(const Vec3& origin) const override;

public:
    Vertex v0, v1, v2;
    Vec3 e1, e2;

    Vec3 face_normal;
    bool one_sided;
    double area;
    std::shared_ptr<Material> mat;
};

inline Triangle::Triangle(const Vertex& vertex0,
                          const Vertex& vertex1,
                          const Vertex& vertex2,
                          std::shared_ptr<Material> material,
                          bool double_sided,
                          bool reset_normal)
    : v0{ vertex0 }
    , v1{ vertex1 }
    , v2{ vertex2 }
    , mat{ material }
    , one_sided{ !double_sided }
{
    e1 = v1.position - v0.position;
    e2 = v2.position - v0.position;

    face_normal = Cross(e1, e2);
    area = face_normal.Normalize() / 2.0;

    if (reset_normal)
    {
        v0.normal = face_normal;
        v1.normal = face_normal;
        v2.normal = face_normal;
    }
};

inline Vec3 Triangle::GetNormal(double u, double v, double w) const
{
    return w * v0.normal + u * v1.normal + v * v2.normal;
}

inline Vec2 Triangle::GetTexCoord(double u, double v, double w) const
{
    return w * v0.texCoords + u * v1.texCoords + v * v2.texCoords;
}

static constexpr Vec3 aabb_offset{ epsilon * 10.0 };

inline bool Triangle::GetAABB(AABB& outAABB) const
{
    outAABB.min = Min(Min(v0.position, v1.position), v2.position) - aabb_offset;
    outAABB.max = Max(Max(v0.position, v1.position), v2.position) + aabb_offset;

    return true;
}

inline double Triangle::EvaluatePDF(const Ray& ray) const
{
    HitRecord rec;
    if (Hit(ray, ray_tolerance, infinity, rec) == false)
    {
        return 0.0;
    }

    return PDFValue(ray, rec);
}

inline double Triangle::PDFValue(const Ray& hit_ray, const HitRecord& hit_rec) const
{
    double distance_squared = hit_rec.t * hit_rec.t * hit_ray.dir.Length2();
    double cosine = fabs(Dot(hit_ray.dir, hit_rec.normal) / hit_ray.dir.Length());

    return distance_squared / (cosine * area);
}

inline Vec3 Triangle::GetRandomDirection(const Vec3& origin) const
{
    double u = Rand(0.0, 0.5);
    double v = Rand(0.0, 0.5);

    Vec3 random_point = v0.position + e1 * u + e2 * v;

    return random_point - origin;
}

} // namespace spt