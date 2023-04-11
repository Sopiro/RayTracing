#include "pathtracer/dielectric.h"

namespace spt
{

bool Dielectric::Scatter(const Ray& in_ray, const HitRecord& in_rec, ScatterRecord& out_srec) const
{
    double refraction_ratio = in_rec.front_face ? (1.0 / ior) : ior;

    Vec3 unit_direction = in_ray.dir.Normalized();

    double cos_theta = Min(Dot(-unit_direction, in_rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    // Check for total internal reflection
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
    out_srec.specular_ray = Ray{ in_rec.point, direction };

    return true;
}

} // namespace spt
