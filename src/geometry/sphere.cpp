#include "spt/sphere.h"

namespace spt
{

bool Sphere::Intersect(Intersection* is, const Ray& ray, f64 t_min, f64 t_max) const
{
    Vec3 oc = ray.origin - center;
    f64 a = ray.dir.Length2();
    f64 half_b = Dot(oc, ray.dir);
    f64 c = oc.Length2() - radius * radius;

    f64 discriminant = half_b * half_b - a * c;
    if (discriminant < 0.0)
    {
        return false;
    }
    f64 sqrt_d = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    f64 root = (-half_b - sqrt_d) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrt_d) / a;
        if (root < t_min || t_max < root)
        {
            return false;
        }
    }

    is->object = this;
    is->t = root;
    is->point = ray.At(root);
    Vec3 outward_normal = (is->point - center) / radius;

    Vec3 t = (fabs(outward_normal.y) > 0.999) ? x_axis : y_axis;
    Vec3 outward_tangent = Cross(t, outward_normal).Normalized();

    SetFaceNormal(is, ray, outward_normal, outward_tangent);
    GetUV(outward_normal, is->uv);

    return true;
}

} // namespace spt