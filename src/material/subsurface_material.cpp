#include "bulbit/bssrdfs.h"
#include "bulbit/bxdfs.h"
#include "bulbit/materials.h"

namespace bulbit
{

bool SubsurfaceMaterial::TestAlpha(const Point2& uv) const
{
    return true;
}
const SpectrumTexture* SubsurfaceMaterial::GetNormalMap() const
{
    return nullptr;
}

Spectrum SubsurfaceMaterial::Le(const Intersection& isect, const Vec3& wo) const
{
    return Spectrum::black;
}

bool SubsurfaceMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    Float eta_p = isect.front_face ? eta : 1 / eta;

    *bsdf = BSDF(
        isect.shading.normal, isect.shading.tangent,
        alloc.new_object<DielectricBxDF>(eta_p, TrowbridgeReitzDistribution(roughness, roughness))
    );

    return true;
}

bool SubsurfaceMaterial::GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    *bssrdf = alloc.new_object<DisneyBSSRDF>(R, d, isect, wo, eta);
    return true;
}

} // namespace bulbit
