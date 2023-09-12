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

    // Found intersection
    is->object = this;
    is->material = material.get();
    is->t = root;
    is->point = ray.At(root);
    Vec3 outward_normal = (is->point - center) / radius;

    Vec3 t = (fabs(outward_normal.y) > 0.999) ? x_axis : y_axis;
    Vec3 outward_tangent = Cross(t, outward_normal).Normalized();

    SetFaceNormal(is, ray, outward_normal, outward_tangent);
    GetUV(outward_normal, is->uv);

    return true;
}

bool Sphere::IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const
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

    return true;
}

Point3 Sphere::Sample(const Point3& ref) const
{
    Vec3 direction = center - ref;
    Real distance = direction.Normalize();
    Real distance_squared = distance * distance;

#if 0
    if (distance * distance <= radius * radius)
    {
        return Sample();
    }
#endif

    Real u1 = Rand();
    Real u2 = Rand();

    Real cos_theta_max = sqrt(1.0 - radius * radius / distance_squared);
    Real z = Real(1.0) + u2 * (cos_theta_max - Real(1.0));

    Real phi = two_pi * u1;

    Real sin_theta = sqrt(Real(1.0) - z * z);
    Real x = cos(phi) * sin_theta;
    Real y = sin(phi) * sin_theta;

    Vec3 d{ x, y, z };

    Real s = distance * z - sqrt(radius * radius - distance_squared * sin_theta * sin_theta);

    // Real cosAlpha = (distance * distance + radius * radius - s * s) / (2 * distance * radius);
    // Real sinAlpha = std::sqrt(std::max((Real)0, 1 - cosAlpha * cosAlpha));

    // Real xx = cos(phi) * sinAlpha;
    // Real yy = sin(phi) * sinAlpha;
    // Real zz = cosAlpha;

    // Vec3 dd{ -xx, -yy, -zz };
    ONB uvw{ direction };
    return ref + uvw.GetLocal(d) * s;
}

} // namespace spt