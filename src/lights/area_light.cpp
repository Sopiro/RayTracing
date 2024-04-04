#include "bulbit/area_light.h"
#include "bulbit/material.h"

namespace bulbit
{

AreaLight::AreaLight(const Ref<Primitive> prim)
    : Light{ Light::Type::area_light }
    , primitive{ prim }
{
}

Spectrum AreaLight::Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref, const Point2& u) const
{
    Intersection sample;
    Vec3 ref2p;
    primitive->Sample(&sample, pdf, &ref2p, ref.point, u);

    *visibility = ref2p.Normalize() - Ray::epsilon;
    *wi = ref2p;

    return material->Emit(sample, ref2p);
}

} // namespace bulbit
