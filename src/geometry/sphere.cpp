#include "bulbit/sphere.h"
#include "bulbit/frame.h"
#include "bulbit/sampling.h"

namespace bulbit
{

bool Sphere::Intersect(Intersection* is, const Ray& ray, Float t_min, Float t_max) const
{
    Vec3 oc = ray.o - center;
    Float a = ray.d.Length2();
    Float half_b = Dot(oc, ray.d);
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
    is->material_index = material;
    is->t = root;
    is->point = ray.At(root);

    Vec3 outward_normal = (is->point - center) / radius;
    Vec3 tangent, bitangent;
    CoordinateSystem(outward_normal, &tangent, &bitangent);

    SetFaceNormal(is, ray.d, outward_normal);
    is->shading.normal = outward_normal;
    is->shading.tangent = tangent;
    is->uv = ComputeTexCoord(outward_normal);

    return true;
}

bool Sphere::IntersectAny(const Ray& ray, Float t_min, Float t_max) const
{
    Vec3 oc = ray.o - center;
    Float a = ray.d.Length2();
    Float half_b = Dot(oc, ray.d);
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

    return true;
}

void Sphere::Sample(Intersection* sample, Float* pdf, const Point2& u) const
{
    Float area = four_pi * radius * radius;
    sample->normal = UniformSampleSphere(u);
    sample->point = center + sample->normal * radius;
    sample->uv = ComputeTexCoord(sample->normal);
    *pdf = 1 / area;
}

void Sphere::Sample(Intersection* sample, Float* pdf, Vec3* ref2p, const Point3& ref, const Point2& u) const
{
    Vec3 direction = center - ref;
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

    *ref2p = frame.FromLocal(d) * s;

    Float solid_angle = two_pi * (1 - cos_theta_max);

    sample->material_index = material;
    sample->point = ref + *ref2p;
    sample->normal = Normalize(sample->point - center);
    *pdf = 1 / solid_angle;

    sample->front_face = true;
}

} // namespace bulbit