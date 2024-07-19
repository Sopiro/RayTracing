#include "bulbit/bxdfs.h"
#include "bulbit/materials.h"

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

DielectricMaterial::DielectricMaterial(Float eta, const FloatTexture* roughness)
    : DielectricMaterial(eta, roughness, roughness)

{
}

DielectricMaterial::DielectricMaterial(Float eta, const FloatTexture* u_roughness, const FloatTexture* v_roughness)
    : Material{ Material::Type::normal }
    , eta{ eta }
    , u_roughness{ u_roughness }
    , v_roughness{ v_roughness }
{
}

bool DielectricMaterial::TestAlpha(const Point2& uv) const
{
    return true;
}

const SpectrumTexture* DielectricMaterial::GetNormalMap() const
{
    return nullptr;
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

} // namespace bulbit
