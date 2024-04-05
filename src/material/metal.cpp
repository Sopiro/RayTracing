#include "bulbit/metal.h"
#include "bulbit/sampling.h"

namespace bulbit
{

Metal::Metal(const Spectrum& albedo, Float fuzziness)
    : albedo{ albedo }
    , fuzziness{ fuzziness }
{
}

bool Metal::Scatter(Interaction* ir, const Intersection& is, const Vec3& wi, const Point2& u) const
{
    Vec3 reflected = Reflect(-wi, is.normal);

    ir->specular_ray.o = is.point;
    ir->specular_ray.d = Normalize(reflected + fuzziness * RandomInUnitSphere(u));
    ir->attenuation = albedo;
    ir->is_specular = true;

    return true;
}

} // namespace bulbit
