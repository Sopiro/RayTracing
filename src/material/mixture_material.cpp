#include "bulbit/hash.h"
#include "bulbit/materials.h"
#include "bulbit/textures.h"

namespace bulbit
{

MixtureMaterial::MixtureMaterial(const Material* material1, const Material* material2, const FloatTexture* amount)
    : Material{ TypeIndexOf<MixtureMaterial>() }
    , mixture_amount{ amount }
{
    BulbitAssert(material1 != nullptr);
    BulbitAssert(material2 != nullptr);
    materials[0] = material1;
    materials[1] = material2;
}

Float MixtureMaterial::GetAlpha(const Intersection& isect) const
{
    if (HashFloat(isect.t, isect.point, isect.normal) > mixture_amount->Evaluate(isect.uv))
    {
        return materials[0]->GetAlpha(isect);
    }
    else
    {
        return materials[1]->GetAlpha(isect);
    }
}

const SpectrumTexture* MixtureMaterial::GetNormalMap() const
{
    BulbitAssert(false);
    return nullptr;
}

Spectrum MixtureMaterial::Le(const Intersection& isect, const Vec3& wo) const
{
    BulbitAssert(false);

    BulbitNotUsed(isect);
    BulbitNotUsed(wo);

    return Spectrum::black;
}

bool MixtureMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    BulbitAssert(false);

    BulbitNotUsed(bsdf);
    BulbitNotUsed(isect);
    BulbitNotUsed(wo);
    BulbitNotUsed(alloc);

    return false;
}

const Material* MixtureMaterial::ChooseMaterial(const Intersection& isect, const Vec3& wo) const
{
    Float m = mixture_amount->Evaluate(isect.uv);
    if (m <= 0)
    {
        return materials[0];
    }
    else if (m >= 1)
    {
        return materials[1];
    }

    Float u = HashFloat(isect.point, wo, materials[0], materials[1]);
    if (m < u)
    {
        return materials[0];
    }
    else
    {
        return materials[1];
    }
}

bool MixtureMaterial::GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    BulbitNotUsed(bssrdf);
    BulbitNotUsed(isect);
    BulbitNotUsed(wo);
    BulbitNotUsed(alloc);
    return false;
}

} // namespace bulbit
