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
    Vec3 normal, tangent;
    NormalMapping(&normal, &tangent, isect);

    Spectrum b = basecolor->Evaluate(isect.uv);
    Float m = metallic->Evaluate(isect.uv);
    Float r = roughness->Evaluate(isect.uv);
    Float alpha = RoughnessToAlpha(r);

    Spectrum f0 = F0(b, m);
    Spectrum F = F_Schlick(f0, Dot(wo, isect.normal));
    Float diff_weight = (1 - m);
    Float spec_weight = F.Luminance();
    // Float spec_weight = std::fmax(F.x, std::fmax(F.y, F.z));
    Float t = Clamp(spec_weight / (diff_weight + spec_weight), Float(0.15), Float(0.9));

    *bsdf = BSDF(normal, tangent, alloc.new_object<UnrealBxDF>(b, m, alpha, t));
    return true;
}

void UnrealMaterial::NormalMapping(Vec3* normal, Vec3* tangent, const Intersection& isect) const
{
    Vec3 sn = ToVector(normalmap->Evaluate(isect.uv)) * 2 - Vec3(1);
    sn.Normalize();

    Frame tbn = Frame::FromZ(isect.shading.normal);
    tbn.z.Normalize();

    *normal = Normalize(tbn.FromLocal(sn));
    *tangent = Cross(tbn.y, *normal);
}

} // namespace bulbit
