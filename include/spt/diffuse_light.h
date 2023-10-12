#include "material.h"

namespace spt
{

class DiffuseLight : public Material
{
public:
    DiffuseLight(const Spectrum& color);
    DiffuseLight(const Ref<Texture> emission);

    virtual bool IsLightSource() const override;
    virtual Spectrum Emit(const Intersection& is, const Vec3& wi) const override;
    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi) const override;

    Ref<Texture> emission;
    bool two_sided;
};

inline DiffuseLight::DiffuseLight(const Spectrum& color)
    : emission{ ConstantColor::Create(color) }
{
}

inline DiffuseLight::DiffuseLight(const Ref<Texture> _emission)
    : emission{ _emission }
    , two_sided{ false }
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
        return emission->Value(is.uv);
    }
    else
    {
        return RGBSpectrum::black;
    }
}

inline bool DiffuseLight::Scatter(Interaction* ir, const Intersection& is, const Vec3& wi) const
{
    return false;
}

} // namespace spt
