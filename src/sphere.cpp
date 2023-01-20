#include "raytracer/sphere.h"

bool Sphere::Hit(const Ray& ray, Real t_min, Real t_max, HitRecord& rec) const
{
    Vec3 oc = ray.origin - center;
    Real a = ray.dir.Length2();
    Real half_b = Dot(oc, ray.dir);
    Real c = oc.Length2() - radius * radius;

    Real discriminant = half_b * half_b - a * c;
    if (discriminant < 0.0)
    {
        return false;
    }
    Real sqrt_d = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    Real root = (-half_b - sqrt_d) / a;
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
    rec.p = ray.At(rec.t);
    Vec3 outward_normal = (rec.p - center) / radius;
    rec.SetFaceNormal(ray, outward_normal);
    GetUV(outward_normal, rec.uv);

    return true;
}