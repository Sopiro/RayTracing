#include "bulbit/bxdfs.h"
#include "bulbit/materials.h"

namespace bulbit
{

ThinDielectricMaterial::ThinDielectricMaterial(Float eta)
    : Material{ Material::Type::normal }
    , eta{ eta }
{
}

bool ThinDielectricMaterial::TestAlpha(const Point2& uv) const
{
    return true;
}

const SpectrumTexture* ThinDielectricMaterial::GetNormalMap() const
{
    return nullptr;
}

Spectrum ThinDielectricMaterial::Le(const Intersection& isect, const Vec3& wo) const
{
    return Spectrum::black;
}

bool ThinDielectricMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    *bsdf = BSDF(isect.shading.normal, isect.shading.tangent, alloc.new_object<ThinDielectricBxDF>(eta));
    return true;
}

} // namespace bulbit
