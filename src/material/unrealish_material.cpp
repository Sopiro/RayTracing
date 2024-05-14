#include "bulbit/bxdfs.h"
#include "bulbit/material.h"
#include "bulbit/microfacet.h"
#include "bulbit/util.h"

namespace bulbit
{

UnrealMaterial::UnrealMaterial(const SpectrumTexture* basecolor,
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

bool UnrealMaterial::TestAlpha(const Point2& uv) const
{
    return basecolor->EvaluateAlpha(uv) > epsilon;
}

Spectrum UnrealMaterial::Le(const Intersection& isect, const Vec3& wi) const
{
    return emissive->Evaluate(isect.uv);
}

bool UnrealMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    Vec3 sn = ToVector(normalmap->Evaluate(isect.uv)) * 2 - Vec3(1);
    sn.Normalize();

    Frame tbn = Frame::FromXZ(isect.shading.tangent, isect.shading.normal);
    tbn.z.Normalize();

    Vec3 normal = Normalize(tbn.FromLocal(sn));
    Vec3 tangent = Cross(tbn.y, normal);

    Spectrum c = basecolor->Evaluate(isect.uv);
    Float m = metallic->Evaluate(isect.uv);
    Float r = roughness->Evaluate(isect.uv);
    Float alpha = RoughnessToAlpha(r);

    Spectrum f0 = F0(c, m);
    Spectrum F = F_Schlick(f0, Dot(wo, isect.normal));
    Float diff_w = (1 - m);
    Float spec_w = F.Luminance();
    // Float spec_w = std::fmax(F.x, std::fmax(F.y, F.z));
    Float t = Clamp(spec_w / (diff_w + spec_w), Float(0.15), Float(0.9));

    *bsdf = BSDF(normal, tangent, alloc.new_object<UnrealBxDF>(c, m, alpha, t));
    return true;
}

} // namespace bulbit
