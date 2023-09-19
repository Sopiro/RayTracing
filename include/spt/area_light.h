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
    virtual f64 EvaluatePDF(const Ray& ray) override;

    const Primitive* GetPrimitive() const;
    Color Emit(const Intersection& is, const Vec3& wi) const;

    bool two_sided;

private:
    Ref<Primitive> primitive;
};

inline AreaLight::AreaLight(const Ref<Primitive> _primitive)
    : Light{ Light::Type::area_light }
    , primitive{ _primitive }
    , two_sided{ false }
{
}

inline Color AreaLight::Sample(Vec3* wi, f64* pdf, f64* visibility, const Intersection& ref) const
{
    Intersection sample;
    Vec3 ref2p;
    primitive->Sample(&sample, pdf, &ref2p, ref.point);

    *visibility = ref2p.Normalize();
    *wi = ref2p;

    if (two_sided || sample.front_face)
    {
        const Material* mat = primitive->GetMaterial();
        return mat->Emit(sample, ref2p);
    }
    else
    {
        return zero_vec3;
    }
}

inline f64 AreaLight::EvaluatePDF(const Ray& ray)
{
    return primitive->EvaluatePDF(ray);
}

inline const Primitive* AreaLight::GetPrimitive() const
{
    return primitive.get();
}

inline Color AreaLight::Emit(const Intersection& is, const Vec3& wi) const
{
    if (two_sided || is.front_face)
    {
        const Material* mat = primitive->GetMaterial();
        return mat->Emit(is, wi);
    }
    else
    {
        return zero_vec3;
    }
}

} // namespace spt
