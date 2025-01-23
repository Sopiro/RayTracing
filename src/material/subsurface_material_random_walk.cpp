#include "bulbit/bssrdfs.h"
#include "bulbit/bxdfs.h"
#include "bulbit/materials.h"
#include "bulbit/textures.h"

namespace bulbit
{

SubsurfaceRandomWalkMaterial::SubsurfaceRandomWalkMaterial(
    const SpectrumTexture* reflectance,
    const Spectrum& mfp,
    Float eta,
    const FloatTexture* u_roughness,
    const FloatTexture* v_roughness,
    Float g,
    const SpectrumTexture* normalmap
)
    : Material(TypeIndexOf<SubsurfaceRandomWalkMaterial>())
    , reflectance{ reflectance }
    , l{ mfp }
    , eta{ eta }
    , u_roughness{ u_roughness }
    , v_roughness{ v_roughness }
    , g{ g }
    , normalmap{ normalmap }
{
}

Float SubsurfaceRandomWalkMaterial::GetAlpha(const Intersection& isect) const
{
    BulbitNotUsed(isect);
    return 1;
}
const SpectrumTexture* SubsurfaceRandomWalkMaterial::GetNormalMap() const
{
    return normalmap;
}

Spectrum SubsurfaceRandomWalkMaterial::Le(const Intersection& isect, const Vec3& wo) const
{
    BulbitNotUsed(isect);
    BulbitNotUsed(wo);
    return Spectrum::black;
}

bool SubsurfaceRandomWalkMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
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

bool SubsurfaceRandomWalkMaterial::GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    Spectrum R = reflectance->Evaluate(isect.uv);
    Spectrum sigma_t = 1 / l;

    *bssrdf = alloc.new_object<RandomWalkBSSRDF>(R, Spectrum(0), sigma_t, isect, wo, eta, g);
    return true;
}

} // namespace bulbit
