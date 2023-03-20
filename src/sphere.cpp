#include "raytracer/sphere.h"

namespace spt
{

bool Sphere::Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const
{
    Vec3 oc = ray.origin - center;
    double a = ray.dir.Length2();
    double half_b = Dot(oc, ray.dir);
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

    rec.object = this;
    rec.mat = material;
    rec.t = root;
    rec.point = ray.At(rec.t);
    Vec3 outward_normal = (rec.point - center) / radius;
    rec.SetFaceNormal(ray, outward_normal);
    GetUV(outward_normal, rec.uv);

    return true;
}

} // namespace spt