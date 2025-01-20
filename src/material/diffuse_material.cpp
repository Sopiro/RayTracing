#include "bulbit/bxdfs.h"
#include "bulbit/materials.h"
#include "bulbit/textures.h"

namespace bulbit
{

DiffuseMaterial::DiffuseMaterial(const Spectrum& albedo, const SpectrumTexture* normalmap, const FloatTexture* alpha)
    : DiffuseMaterial(ColorConstantTexture::Create(albedo), normalmap, alpha)
{
}

DiffuseMaterial::DiffuseMaterial(const SpectrumTexture* albedo, const SpectrumTexture* normalmap, const FloatTexture* alpha)
    : Material{ TypeIndexOf<DiffuseMaterial>() }
    , albedo{ albedo }
    , normalmap{ normalmap }
    , alpha{ alpha }
{
}

bool DiffuseMaterial::TestAlpha(const Point2& uv) const
{
    if (alpha)
    {
        return alpha->Evaluate(uv) > epsilon;
    }
    else
    {
        return true;
    }
}

const SpectrumTexture* DiffuseMaterial::GetNormalMap() const
{
    return normalmap;
}

Spectrum DiffuseMaterial::Le(const Intersection& isect, const Vec3& wo) const
{
    BulbitNotUsed(isect);
    BulbitNotUsed(wo);
    return Spectrum::black;
}

bool DiffuseMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    BulbitNotUsed(wo);
    *bsdf = BSDF(isect.shading.normal, isect.shading.tangent, alloc.new_object<LambertianBxDF>(albedo->Evaluate(isect.uv)));
    return true;
}

bool DiffuseMaterial::GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    BulbitNotUsed(bssrdf);
    BulbitNotUsed(isect);
    BulbitNotUsed(wo);
    BulbitNotUsed(alloc);
    return false;
}

} // namespace bulbit
