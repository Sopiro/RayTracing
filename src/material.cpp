#include "raytracer/material.h"
#include "raytracer/hittable.h"

bool Lambertian::Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const
{
    Vec3 scatter_direction = rec.normal + RandomUnitVector();

    // Catch degenerate scatter direction
    if (scatter_direction.Length2() < DBL_EPSILON * DBL_EPSILON)
    {
        scatter_direction = rec.normal;
    }

    scattered = Ray{ rec.p, scatter_direction };
    attenuation = albedo->Value(rec.uv, rec.p);

    return true;
}

bool Metal::Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const
{
    Vec3 reflected = Reflect(r_in.dir.Normalized(), rec.normal);
    scattered = Ray{ rec.p, reflected + fuzziness * RandomInUnitSphere() };
    attenuation = albedo;

    return Dot(scattered.dir, rec.normal) > 0;
}

bool Dielectric::Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const
{
    attenuation = Color{ 1.0, 1.0, 1.0 };
    double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

    Vec3 unit_direction = r_in.dir.Normalized();

    double cos_theta = fmin(Dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    // Total Internal Reflection
    bool refractable = refraction_ratio * sin_theta < 1.0;
    Vec3 direction;

    if (refractable == false || Reflectance(cos_theta, refraction_ratio) > Rand())
    {
        direction = Reflect(unit_direction, rec.normal);
    }
    else
    {
        direction = Refract(unit_direction, rec.normal, refraction_ratio);
    }

    scattered = Ray{ rec.p, direction };

    return true;
}