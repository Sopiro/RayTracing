#include "bulbit/bxdfs.h"
#include "bulbit/material.h"
#include "bulbit/microfacet.h"
#include "bulbit/util.h"

namespace bulbit
{

UnrealishMaterial::UnrealishMaterial(const SpectrumTexture* basecolor,
                                     const FloatTexture* metallic,
                                     const FloatTexture* roughness,
                                     const SpectrumTexture* emissive,
                                     const SpectrumTexture* normalmap)
    : basecolor{ basecolor }
    , metallic{ metallic }
    , roughness{ roughness }
    , emissive{ emissive }
    , normalmap{ normalmap }
{
}

bool UnrealishMaterial::Scatter(Interaction* ir, const Intersection& isect, const Vec3& wi, const Point2& u) const
{
    ir->bsdf = BSDF(isect.shading.normal, isect.shading.tangent, new (ir->mem) DiffuseBxDF(basecolor->Evaluate(isect.uv)));
    return true;
}

bool UnrealishMaterial::TestAlpha(const Point2& uv) const
{
    return basecolor->EvaluateAlpha(uv) > epsilon;
}

} // namespace bulbit
