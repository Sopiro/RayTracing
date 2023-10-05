#include "spt/sphere.h"
#include "spt/onb.h"
#include "spt/sampling.h"

namespace spt
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
    is->object = this;
    is->material = GetMaterial();
    is->t = root;
    is->point = ray.At(root);
    Vec3 outward_normal = (is->point - center) / radius;

    Vec3 t = (std::fabs(outward_normal.y) > Float(0.999)) ? x_axis : y_axis;
    Vec3 outward_tangent = Normalize(Cross(t, outward_normal));

    SetFaceNormal(is, ray.d, outward_normal);
    is->shading.normal = outward_normal;
    is->shading.tangent = outward_tangent;
    is->uv = ComputeSphereTexCoord(outward_normal);

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

void Sphere::Sample(Intersection* sample, Float* pdf) const
{
    Float area = four_pi * radius * radius;
    sample->normal = UniformSampleSphere();
    sample->point = center + sample->normal * radius;
    sample->uv = ComputeSphereTexCoord(sample->normal);
    *pdf = 1 / area;
}

void Sphere::Sample(Intersection* sample, Float* pdf, Vec3* ref2p, const Point3& ref) const
{
    Vec3 direction = center - ref;
    Float distance = direction.Normalize();
    Float distance_squared = distance * distance;

    Float u1 = Rand();
    Float u2 = Rand();

    Float cos_theta_max = std::sqrt(1 - radius * radius / distance_squared);
    Float z = Float(1.0) + u2 * (cos_theta_max - Float(1.0));

    Float phi = two_pi * u1;

    Float sin_theta = std::sqrt(Float(1.0) - z * z);
    Float x = std::cos(phi) * sin_theta;
    Float y = std::sin(phi) * sin_theta;

    Vec3 d{ x, y, z };

    Float s = distance * z - std::sqrt(radius * radius - distance_squared * sin_theta * sin_theta);

    // Float cosAlpha = (distance * distance + radius * radius - s * s) / (2 * distance * radius);
    // Float sinAlpha = std::std::sqrt(std::fmax((Float)0, 1 - cosAlpha * cosAlpha));

    // Float xx = std::cos(phi) * sinAlpha;
    // Float yy = std::sin(phi) * sinAlpha;
    // Float zz = cosAlpha;

    // Vec3 dd{ -xx, -yy, -zz };
    ONB uvw{ direction };

    *ref2p = uvw.GetLocal(d) * s;

    Float solid_angle = two_pi * (1 - cos_theta_max);

    sample->point = ref + *ref2p;
    sample->normal = Normalize(sample->point - center);
    *pdf = 1 / solid_angle;

    sample->front_face = true;

    sample->object = this;
    sample->material = GetMaterial();
}

} // namespace spt