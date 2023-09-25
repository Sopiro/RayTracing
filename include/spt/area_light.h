#pragma once

#include "light.h"
#include "material.h"
#include "primitive.h"

namespace spt
{

class AreaLight : public Light
{
public:
    AreaLight(const Ref<Primitive> primitive);

    virtual Color Sample(Vec3* wi, f64* pdf, f64* visibility, const Intersection& ref) const override;
    virtual f64 EvaluatePDF(const Ray& ray) const override;

    const Primitive* GetPrimitive() const;

private:
    Ref<Primitive> primitive;
};

inline AreaLight::AreaLight(const Ref<Primitive> _primitive)
    : Light{ Light::Type::area_light }
    , primitive{ _primitive }
{
}

inline Color AreaLight::Sample(Vec3* wi, f64* pdf, f64* visibility, const Intersection& ref) const
{
    Intersection sample;
    Vec3 ref2p;
    primitive->Sample(&sample, pdf, &ref2p, ref.point);

    *visibility = ref2p.Normalize() - ray_epsilon;
    *wi = ref2p;

    return sample.material->Emit(sample, ref2p);
}

inline f64 AreaLight::EvaluatePDF(const Ray& ray) const
{
    return primitive->EvaluatePDF(ray);
}

inline const Primitive* AreaLight::GetPrimitive() const
{
    return primitive.get();
}

} // namespace spt
