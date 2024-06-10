#include "bulbit/bsdf.h"
#include "bulbit/material.h"
#include "bulbit/primitive.h"

namespace bulbit
{

static void NormalMapping(Intersection* isect, const SpectrumTexture* normalmap)
{
    assert(normalmap != nullptr);

    Spectrum sp = normalmap->Evaluate(isect->uv);
    Vec3 sn(sp.r * 2 - 1, sp.g * 2 - 1, sp.b * 2 - 1);
    sn.Normalize();

    Frame tbn = Frame::FromXZ(isect->shading.tangent, isect->shading.normal);

    Vec3 n = tbn.FromLocal(sn);
    Vec3 t = isect->shading.tangent - Dot(isect->shading.tangent, n) * n;

    isect->shading.normal = n;
    isect->shading.tangent = t;
}

Spectrum Intersection::Le(const Vec3& wo) const
{
    return primitive->GetMaterial()->Le(*this, wo);
}

bool Intersection::GetBSDF(BSDF* bsdf, const Vec3& wo, Allocator& alloc)
{
    const Material* mat = primitive->GetMaterial();

    const SpectrumTexture* normalmap = mat->GetNormalMap();
    if (normalmap)
    {
        NormalMapping(this, normalmap);
    }

    return mat->GetBSDF(bsdf, *this, wo, alloc);
}

} // namespace  bulbit
