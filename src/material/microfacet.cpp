#include "bulbit/microfacet.h"
#include "bulbit/material.h"
#include "bulbit/util.h"

namespace bulbit
{

Microfacet::Microfacet(const Texture* basecolor,
                       const Texture* metallic,
                       const Texture* roughness,
                       const Texture* emissive,
                       const Texture* normalmap)
    : basecolor{ basecolor }
    , metallic{ metallic }
    , roughness{ roughness }
    , emissive{ emissive }
    , normalmap{ normalmap }
{
}

Spectrum Microfacet::Emit(const Intersection& is, const Vec3& wi) const
{
    return emissive->Evaluate(is.uv);
}

Spectrum Microfacet::Evaluate(const Intersection& is, const Vec3& wi, const Vec3& wo) const
{
    Vec3 normal = ToVector(normalmap->Evaluate(is.uv)) * 2 - Vec3(1);
    normal.Normalize();

    Frame tbn = Frame::FromXZ(is.shading.tangent, is.shading.normal);
    tbn.z.Normalize();

    Vec3 n = Normalize(tbn.FromLocal(normal)); // normal
    Vec3 v = -wi;                              // incident
    Vec3 l = wo;                               // outgoing
    Vec3 h = v + l;                            // half

    // Resolve back facing shading normal by flipping method
    if (Dot(n, v) < 0)
    {
        n = Reflect(n, is.shading.normal);
    }

    Float NoV = Dot(n, v);
    Float NoL = Dot(n, l);

    if (NoV <= 0 || NoL <= 0 || h == Vec3::zero)
    {
        return RGBSpectrum::black;
    }

    h.Normalize();

    Float NoH = Dot(n, h);
    Float VoH = Dot(v, h);

    Spectrum c = basecolor->Evaluate(is.uv);
    Float m = metallic->Evaluate(is.uv).b;
    Float r = roughness->Evaluate(is.uv).g;

    Float alpha = RoughnessToAlpha(r);
    Float alpha2 = alpha * alpha;

    Spectrum f0 = F0(c, m);
    Spectrum F = F_Schlick(f0, VoH);
    Float D = D_GGX(NoH, alpha2);
    // Float G = G2_Smith_Correlated(NoV, NoL, alpha2);
    Float V = V_Smith_Correlated(NoV, NoL, alpha2);

    Spectrum f_s = F * (D * V);
    // Spectrum f_s = F * (D * G) / (Float(4.0) * NoV * NoL);
    Spectrum f_d = (Spectrum(1) - F) * (1 - m) * (c * inv_pi);

    return (f_d + f_s) * NoL;
}

bool Microfacet::Scatter(Interaction* ir, const Intersection& is, const Vec3& wi, const Point2& u) const
{
    Spectrum c = basecolor->Evaluate(is.uv);
    Float m = metallic->Evaluate(is.uv).b;
    Float r = roughness->Evaluate(is.uv).g;

    Float alpha = RoughnessToAlpha(r);
    Vec3 wo = -wi;

    Spectrum f0 = F0(c, m);
    Spectrum F = F_Schlick(f0, Dot(wo, is.shading.normal));
    Float diff_w = (1 - m);
    Float spec_w = F.Luminance();
    // Float spec_w = std::fmax(F.x, std::fmax(F.y, F.z));
    Float t = Clamp(spec_w / (diff_w + spec_w), Float(0.15), Float(0.9));

    // new (ir->mem) LambertianReflection(is.shading.normal);
    // new (ir->mem) MicrofacetGGX(is.shading.normal, wo, alpha, t);
    new (ir->mem) MicrofacetGGXVNDF(is.shading.normal, wo, alpha, t);
    ir->is_specular = false;

    return true;
}

bool Microfacet::TestAlpha(const Point2& uv) const
{
    return basecolor->EvaluateAlpha(uv) > epsilon;
}

} // namespace bulbit
