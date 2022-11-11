#pragma once

#include "common.h"
#include "hittable.h"
#include "ray.h"

class Sphere : public Hittable
{
public:
    Sphere() = default;
    Sphere(Vec3 c, double r, std::shared_ptr<Material> m)
        : center{ c }
        , radius{ r }
        , material{ m } {};

    virtual bool Hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

public:
    Vec3 center;
    double radius;
    std::shared_ptr<Material> material;

private:
    static void GetUV(const Vec3& p, UV& uv)
    {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        double theta = acos(-p.y);
        double phi = atan2(-p.z, p.x) + pi;

        uv.x = phi / (2.0 * pi);
        uv.y = theta / pi;
    }
};

bool Sphere::Hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const
{
    Vec3 oc = r.origin - center;
    double a = r.dir.Length2();
    double half_b = Dot(oc, r.dir);
    double c = oc.Length2() - radius * radius;

    double discriminant = half_b * half_b - a * c;
    if (discriminant < 0.0)
    {
        return false;
    }
    double sqrt_d = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    double root = (-half_b - sqrt_d) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrt_d) / a;
        if (root < t_min || t_max < root)
        {
            return false;
        }
    }

    rec.mat = material;
    rec.t = root;
    rec.p = r.At(rec.t);
    Vec3 outward_normal = (rec.p - center) / radius;
    rec.SetFaceNormal(r, outward_normal);
    GetUV(outward_normal, rec.uv);

    return true;
}