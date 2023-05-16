#include "spt/sphere.h"

namespace spt
{

bool Sphere::Hit(const Ray& ray, float64 t_min, float64 t_max, HitRecord& rec) const
{
    Vec3 oc = ray.origin - center;
    float64 a = ray.dir.Length2();
    float64 half_b = Dot(oc, ray.dir);
    float64 c = oc.Length2() - radius * radius;

    float64 discriminant = half_b * half_b - a * c;
    if (discriminant < 0.0)
    {
        return false;
    }
    float64 sqrt_d = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    float64 root = (-half_b - sqrt_d) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrt_d) / a;
        if (root < t_min || t_max < root)
        {
            return false;
        }
    }

    rec.object = this;
    rec.mat = material.get();
    rec.t = root;
    rec.point = ray.At(rec.t);
    Vec3 outward_normal = (rec.point - center) / radius;
    Vec3 outward_tangent = Cross(y_axis, rec.normal).Normalized();
    rec.SetFaceNormal(ray, outward_normal, outward_tangent);
    GetUV(outward_normal, rec.uv);

    return true;
}

} // namespace spt