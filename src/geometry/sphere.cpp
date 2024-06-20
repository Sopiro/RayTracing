#include "bulbit/sphere.h"
#include "bulbit/frame.h"
#include "bulbit/material.h"
#include "bulbit/sampling.h"

namespace bulbit
{

bool Sphere::Intersect(Intersection* isect, const Ray& ray, Float t_min, Float t_max) const
{
    Ray r = MulT(transform, ray);

    Vec3 oc = r.o;
    Float a = r.d.Length2();
    Float half_b = Dot(oc, r.d);
    Float c = oc.Length2() - radius * radius;

    Float discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
    {
        return false;
    }
    Float sqrt_d = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    Float root = (-half_b - sqrt_d) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrt_d) / a;
        if (root < t_min || t_max < root)
        {
            return false;
        }
    }

    // Found intersection

    Point3 point = r.At(root);
    Vec3 normal = point / radius;

    Point2 uv = ComputeTexCoord(normal);
    if (material->TestAlpha(uv) == false)
    {
        return false;
    }

    isect->primitive = this;
    isect->t = root;
    isect->point = Mul(transform, point);
    isect->uv = uv;

    Vec3 n = transform.q.Rotate(normal);
    Vec3 t = transform.q.Rotate(Normalize(Cross(y_axis, normal)));
    SetFaceNormal(isect, ray.d, n, n, t);

    return true;
}

bool Sphere::IntersectAny(const Ray& ray, Float t_min, Float t_max) const
{
    Ray r = MulT(transform, ray);

    Vec3 oc = r.o;
    Float a = r.d.Length2();
    Float half_b = Dot(oc, r.d);
    Float c = oc.Length2() - radius * radius;

    Float discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
    {
        return false;
    }
    Float sqrt_d = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    Float root = (-half_b - sqrt_d) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrt_d) / a;
        if (root < t_min || t_max < root)
        {
            return false;
        }
    }

    Point3 point = r.At(root);
    Vec3 normal = point / radius;
    Point2 uv = ComputeTexCoord(normal);

    return material->TestAlpha(uv);
}

PrimitiveSample Sphere::Sample(const Point2& u) const
{
    PrimitiveSample sample;
    sample.normal = UniformSampleSphere(u);
    sample.point = transform.p + sample.normal * radius;

    Float area = four_pi * radius * radius;
    sample.pdf = 1 / area;
    // sample.uv = ComputeTexCoord(sample.normal);

    return sample;
}

PrimitiveSample Sphere::Sample(const Point3& ref, const Point2& u) const
{
    Vec3 direction = transform.p - ref;
    Float distance = direction.Normalize();
    Float distance_squared = distance * distance;

    Float cos_theta_max = std::sqrt(1 - radius * radius / distance_squared);
    Float z = 1 + u[1] * (cos_theta_max - 1);

    Float phi = two_pi * u[0];

    Float sin_theta = std::sqrt(1 - z * z);
    Float x = std::cos(phi) * sin_theta;
    Float y = std::sin(phi) * sin_theta;

    Vec3 d(x, y, z);

    Float s = distance * z - std::sqrt(radius * radius - distance_squared * sin_theta * sin_theta);

    Frame frame(direction);

    Vec3 ref2p = frame.FromLocal(d) * s;

    Float solid_angle = two_pi * (1 - cos_theta_max);

    PrimitiveSample sample;
    sample.point = ref + ref2p;
    sample.normal = Normalize(sample.point - transform.p);
    sample.pdf = 1 / solid_angle;

    return sample;
}

} // namespace bulbit