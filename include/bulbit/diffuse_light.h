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

inline DiffuseLight::DiffuseLight(const Spectrum& color, bool two_sided)
    : emission{ ConstantColor::Create(color) }
    , two_sided{ two_sided }
{
}

inline DiffuseLight::DiffuseLight(const Ref<Texture> emission, bool two_sided)
    : emission{ emission }
    , two_sided{ two_sided }
{
}

inline bool DiffuseLight::IsLightSource() const
{
    return true;
}

inline Spectrum DiffuseLight::Emit(const Intersection& is, const Vec3& wi) const
{
    if (is.front_face || two_sided)
    {
        return emission->Evaluate(is.uv);
    }
    else
    {
        return RGBSpectrum::black;
    }
}

inline bool DiffuseLight::Scatter(Interaction* ir, const Intersection& is, const Vec3& wi, const Point2& u) const
{
    return false;
}

} // namespace bulbit
