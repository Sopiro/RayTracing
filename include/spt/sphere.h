#pragma once

#include "common.h"
#include "hittable.h"
#include "onb.h"
#include "ray.h"

namespace spt
{

class Sphere : public Hittable
{
public:
    Sphere() = default;
    Sphere(const Vec3& center, float64 radius, const Ref<Material>& material);

    virtual bool Hit(const Ray& ray, float64 t_min, float64 t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;
    virtual float64 EvaluatePDF(const Ray& ray) const override;
    virtual float64 PDFValue(const Ray& hit_ray, const HitRecord& hit_rec) const override;
    virtual Vec3 GetRandomDirection(const Point3& origin) const override;
    virtual int32 GetSize() const override;

public:
    Vec3 center;
    float64 radius;
    Ref<Material> material;

private:
    static void GetUV(const Point3& p, UV& out_uv);
};

inline Sphere::Sphere(const Vec3& _center, float64 _radius, const Ref<Material>& _material)
    : center{ _center }
    , radius{ _radius }
    , material{ _material }
{
}

inline bool Sphere::GetAABB(AABB& outAABB) const
{
    outAABB.min = center - Vec3{ radius };
    outAABB.max = center + Vec3{ radius };

    return true;
}

inline float64 Sphere::EvaluatePDF(const Ray& ray) const
{
    HitRecord rec;
    if (Hit(ray, ray_offset, infinity, rec) == false)
    {
        return 0.0;
    }

    return PDFValue(ray, rec);
}

inline float64 Sphere::PDFValue(const Ray& hit_ray, const HitRecord& hit_rec) const
{
    float64 d2 = (center - hit_ray.origin).Length2();
    float64 cos_theta_max = sqrt(1.0 - radius * radius / d2);
    float64 solid_angle = two_pi * (1.0 - cos_theta_max);

    return 1.0 / solid_angle;
}

inline Vec3 Sphere::GetRandomDirection(const Point3& origin) const
{
    Vec3 direction = center - origin;
    float64 distance_sqared = direction.Length2();

    ONB uvw{ direction };

    return uvw.GetLocal(RandomToSphere(radius, distance_sqared));
}

inline int32 Sphere::GetSize() const
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

    float64 theta = acos(-p.y);
    float64 phi = atan2(-p.z, p.x) + pi;

    out_uv.x = phi * inv_two_pi;
    out_uv.y = theta * inv_pi;
}

} // namespace spt