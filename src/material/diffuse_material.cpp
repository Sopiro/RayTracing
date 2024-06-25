#include "bulbit/bxdfs.h"
#include "bulbit/materials.h"

namespace bulbit
{

DiffuseMaterial::DiffuseMaterial(const Spectrum& albedo)
    : Material{ Material::Type::normal }
    , albedo{ ConstantColorTexture::Create(albedo) }
{
}

DiffuseMaterial::DiffuseMaterial(const SpectrumTexture* albedo)
    : Material{ Material::Type::normal }
    , albedo{ albedo }
{
}

bool DiffuseMaterial::TestAlpha(const Point2& uv) const
{
    return albedo->EvaluateAlpha(uv) > epsilon;
}

const SpectrumTexture* DiffuseMaterial::GetNormalMap() const
{
    return nullptr;
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

} // namespace bulbit
