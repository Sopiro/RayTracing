#include "bulbit/bxdfs.h"
#include "bulbit/materials.h"

namespace bulbit
{

DiffuseMaterial::DiffuseMaterial(const Spectrum& albedo, const SpectrumTexture* normalmap)
    : DiffuseMaterial(ConstantColorTexture::Create(albedo), normalmap)
{
}

DiffuseMaterial::DiffuseMaterial(const SpectrumTexture* albedo, const SpectrumTexture* normalmap)
    : Material{ GetTypeIndex<DiffuseMaterial, Materials>() }
    , albedo{ albedo }
    , normalmap{ normalmap }
{
}

bool DiffuseMaterial::TestAlpha(const Point2& uv) const
{
    return albedo->EvaluateAlpha(uv) > epsilon;
}

const SpectrumTexture* DiffuseMaterial::GetNormalMap() const
{
    return normalmap;
}

Spectrum DiffuseMaterial::Le(const Intersection& isect, const Vec3& wo) const
{
    return Spectrum::black;
}

bool DiffuseMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    *bsdf = BSDF(isect.shading.normal, isect.shading.tangent, alloc.new_object<LambertianBxDF>(albedo->Evaluate(isect.uv)));
    return true;
}

bool DiffuseMaterial::GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    return false;
}

} // namespace bulbit
