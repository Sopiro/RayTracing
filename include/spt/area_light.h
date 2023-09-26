#pragma once

#include "light.h"
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

inline f64 AreaLight::EvaluatePDF(const Ray& ray) const
{
    return primitive->EvaluatePDF(ray);
}

inline const Primitive* AreaLight::GetPrimitive() const
{
    return primitive.get();
}

} // namespace spt
