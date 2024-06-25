#include "bulbit/hash.h"
#include "bulbit/material.h"

namespace bulbit
{

MixMaterial::MixMaterial(const Material* material1, const Material* material2, const FloatTexture* amount)
    : Material{ mixture }
    , mixture_amount{ amount }
{
    materials[0] = material1;
    materials[1] = material2;
}

bool MixMaterial::TestAlpha(const Point2& uv) const
{
    assert(false);
    return false;
}

const SpectrumTexture* MixMaterial::GetNormalMap() const
{
    assert(false);
    return nullptr;
}

Spectrum MixMaterial::Le(const Intersection& isect, const Vec3& wo) const
{
    assert(false);
    return Spectrum::black;
}

bool MixMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    assert(false);
    return false;
}

const Material* MixMaterial::ChooseMaterial(const Intersection& isect, const Vec3& wo)
{
    Float m = mixture_amount->Evaluate(isect.uv);
    if (m <= 0)
    {
        return materials[0];
    }
    else if (m >= 1)
    {
        return materials[1];
    }

    Float u = HashFloat(isect.point, wo, materials[0], materials[1]);
    if (m < u)
    {
        return materials[0];
    }
    else
    {
        return materials[1];
    }
}

} // namespace bulbit
