#include "bulbit/bxdfs.h"
#include "bulbit/materials.h"

namespace bulbit
{

MirrorMaterial::MirrorMaterial(const Spectrum& reflectance, const SpectrumTexture* normalmap)
    : MirrorMaterial{ ConstantColorTexture::Create(reflectance), normalmap }
{
}

MirrorMaterial::MirrorMaterial(const SpectrumTexture* reflectance, const SpectrumTexture* normalmap)
    : Material{ GetTypeIndex<MirrorMaterial, Materials>() }
    , reflectance{ reflectance }
    , normalmap{ normalmap }
{
}

bool MirrorMaterial::TestAlpha(const Point2& uv) const
{
    return reflectance->EvaluateAlpha(uv) > epsilon;
}

const SpectrumTexture* MirrorMaterial::GetNormalMap() const
{
    return normalmap;
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

bool MirrorMaterial::GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    return false;
}

} // namespace bulbit
