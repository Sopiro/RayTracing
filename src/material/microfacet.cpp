#include "bulbit/microfacet.h"
#include "bulbit/bxdfs.h"
#include "bulbit/material.h"
#include "bulbit/util.h"

namespace bulbit
{

Microfacet::Microfacet(const SpectrumTexture* basecolor,
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

bool Microfacet::Scatter(Interaction* ir, const Intersection& is, const Vec3& wi, const Point2& u) const
{
    new (ir->mem) DiffuseBxDF(basecolor->Evaluate(is.uv));
    ir->bsdf = BSDF(is.shading.normal, is.shading.tangent, (BxDF*)ir->mem);

    return true;
}

bool Microfacet::TestAlpha(const Point2& uv) const
{
    return basecolor->EvaluateAlpha(uv) > epsilon;
}

} // namespace bulbit
