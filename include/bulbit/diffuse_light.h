#pragma once

#include "material.h"

namespace bulbit
{

class DiffuseLight : public Material
{
public:
    DiffuseLight(const Spectrum& color, bool two_sided = false);
    DiffuseLight(const Ref<Texture> emission, bool two_sided = false);

    virtual bool IsLightSource() const override;
    virtual Spectrum Emit(const Intersection& is, const Vec3& wi) const override;
    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const override;

    Ref<Texture> emission;
    bool two_sided;
};

} // namespace bulbit
