#include "bulbit/bxdfs.h"
#include "bulbit/materials.h"
#include "bulbit/textures.h"

namespace bulbit
{

DiffuseLightMaterial::DiffuseLightMaterial(const Spectrum& color, bool two_sided, const FloatTexture* alpha)
    : DiffuseLightMaterial(ColorConstantTexture::Create(color), two_sided, alpha)
{
}

DiffuseLightMaterial::DiffuseLightMaterial(const SpectrumTexture* emission, bool two_sided, const FloatTexture* alpha)
    : Material{ TypeIndexOf<DiffuseLightMaterial>() }
    , two_sided{ two_sided }
    , emission{ emission }
    , alpha{ alpha }
{
}

bool DiffuseLightMaterial::TestAlpha(const Point2& uv) const
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

const SpectrumTexture* DiffuseLightMaterial::GetNormalMap() const
{
    return nullptr;
}

Spectrum DiffuseLightMaterial::Le(const Intersection& isect, const Vec3& wo) const
{
    BulbitNotUsed(wo);

    if (!TestAlpha(isect.uv))
    {
        return Spectrum::black;
    }

    if (isect.front_face || two_sided)
    {
        return emission->Evaluate(isect.uv);
    }
    else
    {
        return Spectrum::black;
    }
}

bool DiffuseLightMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    BulbitNotUsed(wo);

    *bsdf = BSDF(isect.shading.normal, isect.shading.tangent, alloc.new_object<LambertianBxDF>(Spectrum::black));
    return true;
}

bool DiffuseLightMaterial::GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    BulbitNotUsed(bssrdf);
    BulbitNotUsed(isect);
    BulbitNotUsed(wo);
    BulbitNotUsed(alloc);
    return false;
}

} // namespace bulbit
