#include "bulbit/bxdfs.h"
#include "bulbit/material.h"

namespace bulbit
{

MirrorMaterial::MirrorMaterial(const Spectrum& reflectance)
    : reflectance{ ConstantColorTexture::Create(reflectance) }
{
}

MirrorMaterial::MirrorMaterial(const SpectrumTexture* reflectance)
    : reflectance{ reflectance }
{
}

bool MirrorMaterial::TestAlpha(const Point2& uv) const
{
    return reflectance->EvaluateAlpha(uv) > epsilon;
}

const SpectrumTexture* MirrorMaterial::GetNormalMap() const
{
    return nullptr;
}

Spectrum MirrorMaterial::Le(const Intersection& isect, const Vec3& wo) const
{
    return Spectrum::black;
}

bool MirrorMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    Spectrum r = reflectance->Evaluate(isect.uv);
    *bsdf = BSDF(isect.shading.normal, isect.shading.tangent, alloc.new_object<SpecularReflectionBxDF>(r));
    return true;
}

} // namespace bulbit
