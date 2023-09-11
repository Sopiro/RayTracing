#pragma once

#include "common.h"
#include "intersectable.h"
#include "onb.h"
#include "ray.h"

namespace spt
{

class Sphere : public Intersectable
{
public:
    Sphere() = default;
    Sphere(const Vec3& center, f64 radius, const Ref<Material>& material);

    virtual bool Intersect(Intersection* out_is, const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual bool IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual bool GetAABB(AABB* out_aabb) const override;
    virtual Point3 Sample() const override;
    virtual Point3 Sample(const Point3& ref) const override;
    virtual f64 EvaluatePDF(const Ray& ray) const override;
    virtual f64 PDFValue(const Intersection& hit_is, const Ray& hit_ray) const override;
    virtual i32 GetSize() const override;
    virtual const Material* GetMaterial() const override;

public:
    Vec3 center;
    f64 radius;
    Ref<Material> material;

private:
    static void GetUV(const Point3& p, UV& out_uv);
};

inline Sphere::Sphere(const Vec3& _center, f64 _radius, const Ref<Material>& _material)
    : center{ _center }
    , radius{ _radius }
    , material{ _material }
{
}

inline bool Sphere::GetAABB(AABB* out_aabb) const
{
    out_aabb->min = center - Vec3{ radius };
    out_aabb->max = center + Vec3{ radius };

    return true;
}

inline Point3 Sphere::Sample() const
{
    return center + UniformSampleSphere() * radius;
}

inline f64 Sphere::EvaluatePDF(const Ray& ray) const
{
    Intersection is;
    if (Intersect(&is, ray, ray_offset, infinity) == false)
    {
        return 0.0;
    }

    return PDFValue(is, ray);
}

inline f64 Sphere::PDFValue(const Intersection& hit_is, const Ray& hit_ray) const
{
    f64 distance_squared = (center - hit_ray.origin).Length2();
    f64 cos_theta_max = sqrt(1.0 - radius * radius / distance_squared);
    f64 solid_angle = two_pi * (1.0 - cos_theta_max);

    return 1.0 / solid_angle;
}

inline i32 Sphere::GetSize() const
{
    return 1;
}

inline void Sphere::GetUV(const Point3& p, UV& out_uv)
{
    // p: a given point on the sphere of radius one, centered at the origin.
    // u: returned value [0,1] of angle around the Y axis from X=-1.
    // v: returned value [0,1] of angle from Y=-1 to Y=+1.
    //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
    //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

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