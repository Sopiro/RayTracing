#include "spt/dielectric.h"

namespace spt
{

bool Dielectric::Scatter(const Ray& in_wi, const Intersection& in_is, Interaction& out_ir) const
{
    f64 refraction_ratio = in_is.front_face ? (1.0 / ior) : ior;

    Vec3 unit_direction = in_wi.dir.Normalized();

    f64 cos_theta = Min(Dot(-unit_direction, in_is.normal), 1.0);
    f64 sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    // Check for total internal reflection
    bool refractable = refraction_ratio * sin_theta < 1.0;
    Vec3 direction;

    if (refractable == false || Reflectance(cos_theta, refraction_ratio) > Rand())
    {
        direction = Reflect(unit_direction, in_is.normal);
    }
    else
    {
        direction = Refract(unit_direction, in_is.normal, refraction_ratio);
    }

    out_ir.is_specular = true;
    out_ir.pdf = nullptr;
    out_ir.attenuation = Color{ 1.0, 1.0, 1.0 };
    out_ir.specular_ray = Ray{ in_is.point, direction };

    return true;
}

} // namespace spt
