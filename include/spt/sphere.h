#pragma once

#include "common.h"
#include "onb.h"
#include "primitive.h"
#include "ray.h"

namespace spt
{

class Sphere : public Primitive
{
public:
    Sphere() = default;
    Sphere(const Vec3& center, f64 radius, const Ref<Material> material);

    virtual bool Intersect(Intersection* out_is, const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual bool IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual void GetAABB(AABB* out_aabb) const override;

    virtual void Sample(SurfaceSample* sample) const override;
    virtual void Sample(SurfaceSample* sample, Vec3* ref2p, const Point3& ref) const override;
    virtual f64 EvaluatePDF(const Ray& ray) const override;

    virtual const Material* GetMaterial() const override;

public:
    Vec3 center;
    f64 radius;
    Ref<Material> material;

private:
    static void GetUV(const Point3& p, UV& out_uv);
};

inline Sphere::Sphere(const Vec3& _center, f64 _radius, const Ref<Material> _material)
    : center{ _center }
    , radius{ _radius }
    , material{ _material }
{
}

inline void Sphere::GetAABB(AABB* out_aabb) const
{
    out_aabb->min = center - Vec3{ radius };
    out_aabb->max = center + Vec3{ radius };
}

inline void Sphere::Sample(SurfaceSample* sample) const
{
    f64 area = 4.0 * pi * radius * radius;
    sample->n = UniformSampleSphere();
    sample->p = center + sample->n * radius;
    sample->pdf = 1.0 / area;
}

inline f64 Sphere::EvaluatePDF(const Ray& ray) const
{
    Intersection is;
    if (Intersect(&is, ray, ray_offset, infinity) == false)
    {
        return 0.0;
    }

    f64 distance_squared = (center - ray.origin).Length2();
    f64 cos_theta_max = sqrt(1.0 - radius * radius / distance_squared);
    f64 solid_angle = two_pi * (1.0 - cos_theta_max);

    return 1.0 / solid_angle;
}

inline void Sphere::GetUV(const Point3& p, UV& out_uv)
{
    f64 theta = acos(-p.y);
    f64 phi = atan2(-p.z, p.x) + pi;

    out_uv.x = phi * inv_two_pi;
    out_uv.y = theta * inv_pi;
}

inline const Material* Sphere::GetMaterial() const
{
    return material.get();
}

} // namespace spt