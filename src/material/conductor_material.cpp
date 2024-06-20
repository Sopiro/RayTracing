#include "bulbit/bxdfs.h"
#include "bulbit/material.h"

namespace bulbit
{

ConductorMaterial::ConductorMaterial(const SpectrumTexture* eta,
                                     const SpectrumTexture* k,
                                     const FloatTexture* u_roughness,
                                     const FloatTexture* v_roughness)
    : eta{ eta }
    , k{ k }
    , u_roughness{ u_roughness }
    , v_roughness{ v_roughness }
{
}

bool ConductorMaterial::TestAlpha(const Point2& uv) const
{
    return true;
}

const SpectrumTexture* ConductorMaterial::GetNormalMap() const
{
    return nullptr;
}

Spectrum ConductorMaterial::Le(const Intersection& isect, const Vec3& wo) const
{
    return Spectrum::black;
}

bool ConductorMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    Float alpha_x = u_roughness->Evaluate(isect.uv);
    Float alpha_y = v_roughness->Evaluate(isect.uv);

    Spectrum eta_s = eta->Evaluate(isect.uv);
    Spectrum k_s = k->Evaluate(isect.uv);

    *bsdf = BSDF(isect.shading.normal, isect.shading.tangent,
                 alloc.new_object<ConductorBxDF>(eta_s, k_s, TrowbridgeReitzDistribution(alpha_x, alpha_y)));
    return true;
}

} // namespace bulbit
