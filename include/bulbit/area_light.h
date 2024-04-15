#pragma once

#include "light.h"
#include "primitive.h"

namespace bulbit
{

class Material;

class AreaLight : public Light
{
public:
    AreaLight(const Primitive* primitive);

    virtual Light* Clone(Allocator* allocator) const override;

    virtual Spectrum Sample(Vec3* wi, Float* pdf, Float* visibility, const Intersection& ref, const Point2& u) const override;
    virtual Float EvaluatePDF(const Ray& ray) const override;

    const Primitive* GetPrimitive() const;

private:
    friend class Scene;

    const Primitive* primitive;
    const Material* material;
};

inline Float AreaLight::EvaluatePDF(const Ray& ray) const
{
    return primitive->EvaluatePDF(ray);
}

inline const Primitive* AreaLight::GetPrimitive() const
{
    return primitive;
}

} // namespace bulbit
