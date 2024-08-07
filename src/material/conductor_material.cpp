#include "bulbit/bxdfs.h"
#include "bulbit/materials.h"

namespace bulbit
{

ConductorMaterial::ConductorMaterial(const SpectrumTexture* eta, const SpectrumTexture* k, const FloatTexture* roughness)
    : ConductorMaterial(eta, k, roughness, roughness)
{
}

ConductorMaterial::ConductorMaterial(
    const SpectrumTexture* eta,
    const SpectrumTexture* k,
    const FloatTexture* u_roughness,
    const FloatTexture* v_roughness,
    const SpectrumTexture* normalmap
)
    : Material{ GetTypeIndex<ConductorMaterial, Materials>() }
    , eta{ eta }
    , k{ k }
    , u_roughness{ u_roughness }
    , v_roughness{ v_roughness }
    , normalmap{ normalmap }
{
}

ConductorMaterial::ConductorMaterial(const SpectrumTexture* reflectance, const FloatTexture* roughness)
    : ConductorMaterial(reflectance, roughness, roughness)
{
}

ConductorMaterial::ConductorMaterial(
    const SpectrumTexture* reflectance,
    const FloatTexture* u_roughness,
    const FloatTexture* v_roughness,
    const SpectrumTexture* normalmap
)
    : Material{ GetTypeIndex<ConductorMaterial, Materials>() }
    , eta{ nullptr }
    , k{ reflectance }
    , u_roughness{ u_roughness }
    , v_roughness{ v_roughness }
    , normalmap{ normalmap }
{
}

bool ConductorMaterial::TestAlpha(const Point2& uv) const
{
    return true;
}

const SpectrumTexture* ConductorMaterial::GetNormalMap() const
{
    return normalmap;
}

Spectrum ConductorMaterial::Le(const Intersection& isect, const Vec3& wo) const
{
    return Spectrum::black;
}

bool ConductorMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    Spectrum eta_s, k_s;
    if (eta)
    {
        eta_s = eta->Evaluate(isect.uv);
        k_s = k->Evaluate(isect.uv);
    }
    else
    {
        // The reflectance R for a conductor is:
        // R = \frac{(\eta - 1)^2 + k^2}{(\eta + 1)^2 + k^2}
        // Assume \eta = 1 and solve for k

        eta_s = Spectrum(1.0f);
        Spectrum r = Clamp(k->Evaluate(isect.uv), 0, 0.9999);
        k_s = 2 * Sqrt(r) / Sqrt(Max(Spectrum(1) - r, 0));
    }

    Float alpha_x = u_roughness->Evaluate(isect.uv);
    Float alpha_y = v_roughness->Evaluate(isect.uv);

    *bsdf = BSDF(
        isect.shading.normal, isect.shading.tangent,
        alloc.new_object<ConductorBxDF>(eta_s, k_s, TrowbridgeReitzDistribution(alpha_x, alpha_y))
    );
    return true;
}

bool ConductorMaterial::GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    return false;
}

} // namespace bulbit
