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
    Sphere(Vec3 _center, double _radius, std::shared_ptr<Material> _material)
        : center{ _center }
        , radius{ _radius }
        , material{ _material } {};

    virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;
    virtual double EvaluatePDF(const Vec3& origin, const Vec3& dir) const override;
    virtual double PDFValue(const Vec3& origin, const Vec3& dir, const HitRecord& rec) const override;
    virtual Vec3 GetRandomDirection(const Vec3& origin) const override;

public:
    Vec3 center;
    double radius;
    std::shared_ptr<Material> material;

private:
    static void GetUV(const Vec3& p, UV& out_uv)
    {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        double theta = acos(-p.y);
        double phi = atan2(-p.z, p.x) + pi;

        out_uv.x = phi / (2.0 * pi);
        out_uv.y = theta / pi;
    }
};

inline bool Sphere::GetAABB(AABB& outAABB) const
{
    outAABB.min = center - Vec3{ radius };
    outAABB.max = center + Vec3{ radius };

    return true;
}

inline double Sphere::EvaluatePDF(const Vec3& origin, const Vec3& dir) const
{
    HitRecord rec;
    if (Hit(Ray{ origin, dir }, ray_tolerance, infinity, rec) == false)
    {
        return 0.0;
    }

    double cos_theta_max = sqrt(1.0 - radius * radius / (center - origin).Length2());
    double solid_angle = 2.0 * pi * (1.0 - cos_theta_max);

    return 1.0 / solid_angle;
}

inline double Sphere::PDFValue(const Vec3& origin, const Vec3& dir, const HitRecord& rec) const
{
    double cos_theta_max = sqrt(1.0 - radius * radius / (center - origin).Length2());
    double solid_angle = 2.0 * pi * (1.0 - cos_theta_max);

    return 1.0 / solid_angle;
}

inline Vec3 Sphere::GetRandomDirection(const Vec3& origin) const
{
    Vec3 direction = center - origin;
    double distance_sqared = direction.Length2();

    ONB uvw{ direction };

    return uvw.GetLocal(RandomToSphere(radius, distance_sqared));
}

} // namespace spt