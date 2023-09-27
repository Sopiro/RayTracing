#include "spt/area_light.h"
#include "spt/material.h"

namespace spt
{

AreaLight::AreaLight(const Ref<Primitive> _primitive)
    : Light{ Light::Type::area_light }
    , primitive{ _primitive }
{
}

Color AreaLight::Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref) const
{
    Intersection sample;
    Vec3 ref2p;
    primitive->Sample(&sample, pdf, &ref2p, ref.point);

    *visibility = ref2p.Normalize() - Ray::epsilon;
    *wi = ref2p;

    return sample.material->Emit(sample, ref2p);
}

} // namespace spt
