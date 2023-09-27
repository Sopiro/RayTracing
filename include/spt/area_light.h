#pragma once

#include "light.h"
#include "primitive.h"

namespace spt
{

class AreaLight : public Light
{
public:
    AreaLight(const Ref<Primitive> primitive);

    virtual Color Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref) const override;
    virtual Float EvaluatePDF(const Ray& ray) const override;

    const Primitive* GetPrimitive() const;

private:
    Ref<Primitive> primitive;
};

inline Float AreaLight::EvaluatePDF(const Ray& ray) const
{
    return primitive->EvaluatePDF(ray);
}

inline const Primitive* AreaLight::GetPrimitive() const
{
    return primitive.get();
}

} // namespace spt
