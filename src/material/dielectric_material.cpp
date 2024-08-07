#include "bulbit/bxdfs.h"
#include "bulbit/materials.h"
#include "bulbit/textures.h"

namespace bulbit
{

DielectricMaterial::DielectricMaterial(Float eta)
    : DielectricMaterial(eta, ConstantFloatTexture::Create(0))
{
}

DielectricMaterial::DielectricMaterial(Float eta, Float roughness)
    : DielectricMaterial(eta, ConstantFloatTexture::Create(roughness))
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
    , eta{ eta }
    , u_roughness{ u_roughness }
    , v_roughness{ v_roughness }
    , normalmap{ normalmap }
{
}

bool DielectricMaterial::TestAlpha(const Point2& uv) const
{
    return true;
}

const SpectrumTexture* DielectricMaterial::GetNormalMap() const
{
    return normalmap;
}

Spectrum DielectricMaterial::Le(const Intersection& isect, const Vec3& wo) const
{
    return Spectrum::black;
}

bool DielectricMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
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
    return false;
}

} // namespace bulbit
