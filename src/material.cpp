#include "raytracer/material.h"
#include "raytracer/hittable.h"
#include "raytracer/onb.h"
#include "raytracer/pdf.h"

namespace spt
{

bool Lambertian::Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const
{
    out_srec.is_specular = false;
    out_srec.attenuation = albedo->Value(in_rec.uv, in_rec.p);
    out_srec.pdf = std::make_shared<CosinePDF>(in_rec.normal);

    return true;
}

double Lambertian::ScatteringPDF(const Ray& in_ray, const HitRecord& in_rec, const Ray& in_scattered) const
{
    // Cosine density
    return Dot(in_rec.normal, in_scattered.dir) / pi;
}

bool Metal::Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const
{
    Vec3 reflected = Reflect(in_ray.dir.Normalized(), in_rec.normal);

    out_srec.specular_ray = Ray{ in_rec.p, reflected + fuzziness * RandomInUnitSphere() };
    out_srec.attenuation = albedo;
    out_srec.is_specular = true;
    out_srec.pdf = nullptr;

    return true;
}

bool Dielectric::Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const
{
    double refraction_ratio = in_rec.front_face ? (1.0 / ir) : ir;

    Vec3 unit_direction = in_ray.dir.Normalized();

    double cos_theta = Min(Dot(-unit_direction, in_rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    // Total Internal Reflection
    bool refractable = refraction_ratio * sin_theta < 1.0;
    Vec3 direction;

    if (refractable == false || Reflectance(cos_theta, refraction_ratio) > Rand())
    {
        direction = Reflect(unit_direction, in_rec.normal);
    }
    else
    {
        direction = Refract(unit_direction, in_rec.normal, refraction_ratio);
    }

    out_srec.is_specular = true;
    out_srec.pdf = nullptr;
    out_srec.attenuation = Color{ 1.0, 1.0, 1.0 };
    out_srec.specular_ray = Ray{ in_rec.p, direction };

    return true;
}

bool Isotropic::Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const
{
    out_srec.specular_ray = Ray{ in_rec.p, RandomInUnitSphere() };
    out_srec.attenuation = albedo->Value(in_rec.uv, in_rec.p);

    return true;
}

Color DiffuseLight::Emit(const Ray& in_ray, const HitRecord& in_rec) const
{
    if (in_rec.front_face)
    {
        return emit->Value(in_rec.uv, in_rec.p);
    }
    else
    {
        return Color{ 0.0, 0.0, 0.0 };
    }
}

} // namespace spt