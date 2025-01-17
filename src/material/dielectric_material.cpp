#include "bulbit/bxdfs.h"
#include "bulbit/materials.h"
#include "bulbit/textures.h"

namespace bulbit
{

DielectricMaterial::DielectricMaterial(Float eta)
    : DielectricMaterial(eta, FloatConstantTexture::Create(0))
{
}

DielectricMaterial::DielectricMaterial(Float eta, Float roughness)
    : DielectricMaterial(eta, FloatConstantTexture::Create(roughness))
{
}

DielectricMaterial::DielectricMaterial(Float eta, const FloatTexture* roughness, const SpectrumTexture* normalmap)
    : DielectricMaterial(eta, roughness, roughness, normalmap)

{
}

DielectricMaterial::DielectricMaterial(
    Float eta, const FloatTexture* u_roughness, const FloatTexture* v_roughness, const SpectrumTexture* normalmap
)
    : Material{ TypeIndexOf<DielectricMaterial>() }
    , normalmap{ normalmap }
    , u_roughness{ u_roughness }
    , v_roughness{ v_roughness }
    , eta{ eta }
{
}

bool DielectricMaterial::TestAlpha(const Point2& uv) const
{
    BulbitNotUsed(uv);
    return true;
}

const SpectrumTexture* DielectricMaterial::GetNormalMap() const
{
    return normalmap;
}

Spectrum DielectricMaterial::Le(const Intersection& isect, const Vec3& wo) const
{
    BulbitNotUsed(isect);
    BulbitNotUsed(wo);
    return Spectrum::black;
}

bool DielectricMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    BulbitNotUsed(wo);

    Float alpha_x = u_roughness->Evaluate(isect.uv);
    Float alpha_y = v_roughness->Evaluate(isect.uv);

    Float eta_p = isect.front_face ? eta : 1 / eta;

    *bsdf = BSDF(
        isect.shading.normal, isect.shading.tangent,
        alloc.new_object<DielectricBxDF>(eta_p, TrowbridgeReitzDistribution(alpha_x, alpha_y))
    );
    return true;
}

bool DielectricMaterial::GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    BulbitNotUsed(bssrdf);
    BulbitNotUsed(isect);
    BulbitNotUsed(wo);
    BulbitNotUsed(alloc);
    return false;
}

} // namespace bulbit
