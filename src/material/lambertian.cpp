#include "bulbit/lambertian.h"

namespace bulbit
{

Lambertian::Lambertian(const Spectrum& color)
    : albedo{ ConstantColor::Create(color) }
{
}

Lambertian::Lambertian(const Ref<Texture> albedo)
    : albedo{ albedo }
{
}

bool Lambertian::Scatter(Interaction* ir, const Intersection& is, const Vec3& wi, const Point2& u) const
{
    ir->is_specular = false;
    new (ir->mem) LambertianReflection(is.normal);

    return true;
}

Spectrum Lambertian::Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const
{
    return albedo->Evaluate(is.uv) * Dot(is.normal, wo) * inv_pi;
}

} // namespace bulbit
