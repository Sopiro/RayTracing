#include "spt/dielectric.h"

namespace spt
{

Dielectric::Dielectric(Float index_of_refraction)
    : ior{ index_of_refraction }
{
}

bool Dielectric::Scatter(Interaction* ir, const Intersection& is, const Vec3& wi) const
{
    Float refraction_ratio = is.front_face ? (1 / ior) : ior;

    Float cos_theta = std::fmin(Dot(-wi, is.normal), Float(1.0));
    Float sin_theta = std::sqrt(1 - cos_theta * cos_theta);

    // Check for total internal reflection
    bool refractable = refraction_ratio * sin_theta < 1;
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
    ir->attenuation = Color(1, 1, 1);
    ir->specular_ray = Ray{ is.point, wo };

    return true;
}

} // namespace spt
