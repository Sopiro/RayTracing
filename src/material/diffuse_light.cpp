#include "bulbit/material.h"

namespace bulbit
{

DiffuseLight::DiffuseLight(const Spectrum& color, bool two_sided)
    : emission{ ConstantColorTexture::Create(color) }
    , two_sided{ two_sided }
{
}

DiffuseLight::DiffuseLight(const SpectrumTexture* emission, bool two_sided)
    : emission{ emission }
    , two_sided{ two_sided }
{
}

bool DiffuseLight::IsLightSource() const
{
    return true;
}

Spectrum DiffuseLight::Le(const Intersection& isect, const Vec3& wi) const
{
    if (isect.front_face || two_sided)
    {
        return emission->Evaluate(isect.uv);
    }
    else
    {
        return Spectrum::black;
    }
}

bool DiffuseLight::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wi, const Point2& u, Allocator& alloc) const
{
    return false;
}

} // namespace bulbit
