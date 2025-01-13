#include "bulbit/bssrdfs.h"
#include "bulbit/bxdfs.h"
#include "bulbit/materials.h"
#include "bulbit/textures.h"

namespace bulbit
{

SubsurfaceMaterialRandomWalk::SubsurfaceMaterialRandomWalk(
    const Spectrum& reflectance, const Spectrum& mfp, Float eta, Float roughness, Float g, const SpectrumTexture* normalmap
)
    : SubsurfaceMaterialRandomWalk(
          ConstantColorTexture::Create(reflectance), mfp, eta, ConstantFloatTexture::Create(roughness), g, normalmap
      )
{
}

SubsurfaceMaterialRandomWalk::SubsurfaceMaterialRandomWalk(
    const SpectrumTexture* reflectance, const Spectrum& mfp, Float eta, Float roughness, Float g, const SpectrumTexture* normalmap
)
    : SubsurfaceMaterialRandomWalk(reflectance, mfp, eta, ConstantFloatTexture::Create(roughness), g, normalmap)
{
}

SubsurfaceMaterialRandomWalk::SubsurfaceMaterialRandomWalk(
    const SpectrumTexture* reflectance,
    const Spectrum& mfp,
    Float eta,
    const FloatTexture* roughness,
    Float g,
    const SpectrumTexture* normalmap
)
    : SubsurfaceMaterialRandomWalk(reflectance, mfp, eta, roughness, roughness, g, normalmap)
{
}

SubsurfaceMaterialRandomWalk::SubsurfaceMaterialRandomWalk(
    const SpectrumTexture* reflectance,
    const Spectrum& mfp,
    Float eta,
    const FloatTexture* u_roughness,
    const FloatTexture* v_roughness,
    Float g,
    const SpectrumTexture* normalmap
)
    : Material(TypeIndexOf<SubsurfaceMaterialRandomWalk>())
    , normalmap{ normalmap }
    , u_roughness{ u_roughness }
    , v_roughness{ v_roughness }
    , reflectance{ reflectance }
    , l{ mfp }
    , eta{ eta }
    , g{ g }
{
}

bool SubsurfaceMaterialRandomWalk::TestAlpha(const Point2& uv) const
{
    BulbitNotUsed(uv);
    return true;
}
const SpectrumTexture* SubsurfaceMaterialRandomWalk::GetNormalMap() const
{
    return normalmap;
}

Spectrum SubsurfaceMaterialRandomWalk::Le(const Intersection& isect, const Vec3& wo) const
{
    BulbitNotUsed(isect);
    BulbitNotUsed(wo);
    return Spectrum::black;
}

bool SubsurfaceMaterialRandomWalk::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
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

bool SubsurfaceMaterialRandomWalk::GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    Spectrum R = reflectance->Evaluate(isect.uv);
    Spectrum sigma_t = 1 / l;

    *bssrdf = alloc.new_object<RandomWalkBSSRDF>(R, Spectrum(0), sigma_t, isect, wo, eta, g);
    return true;
}

} // namespace bulbit
