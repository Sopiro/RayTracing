#include "bulbit/diffuse_light.h"

namespace bulbit
{

DiffuseLight::DiffuseLight(const Spectrum& color, bool two_sided)
    : emission{ ConstantColor::Create(color) }
    , two_sided{ two_sided }
{
}

DiffuseLight::DiffuseLight(const Ref<Texture> emission, bool two_sided)
    : emission{ emission }
    , two_sided{ two_sided }
{
}

bool DiffuseLight::IsLightSource() const
{
    return true;
}

Spectrum DiffuseLight::Emit(const Intersection& is, const Vec3& wi) const
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

bool DiffuseLight::Scatter(Interaction* ir, const Intersection& is, const Vec3& wi, const Point2& u) const
{
    return false;
}

} // namespace bulbit
