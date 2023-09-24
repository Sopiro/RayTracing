#include "spt/dielectric.h"

namespace spt
{

bool Dielectric::Scatter(Interaction* ir, const Intersection& is, const Vec3& wi) const
{
    f64 refraction_ratio = is.front_face ? (1.0 / ior) : ior;

    f64 cos_theta = std::fmin(Dot(-wi, is.normal), 1.0);
    f64 sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    // Check for total internal reflection
    bool refractable = refraction_ratio * sin_theta < 1.0;
    Vec3 wo;

    if (refractable == false || Reflectance(cos_theta, refraction_ratio) > Rand())
    {
        wo = Reflect(-wi, is.normal);
    }
    else
    {
        wo = Refract(-wi, is.normal, refraction_ratio);
    }

    ir->is_specular = true;
    ir->attenuation = Color(1.0, 1.0, 1.0);
    ir->specular_ray = Ray{ is.point, wo };

    return true;
}

} // namespace spt
