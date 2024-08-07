#include "bulbit/hash.h"
#include "bulbit/materials.h"

namespace bulbit
{

MixtureMaterial::MixtureMaterial(const Material* material1, const Material* material2, Float mix)
    : MixtureMaterial(material1, material2, ConstantFloatTexture::Create(mix))
{
}

MixtureMaterial::MixtureMaterial(const Material* material1, const Material* material2, const FloatTexture* amount)
    : Material{ TypeIndex<MixtureMaterial>() }
    , mixture_amount{ amount }
{
    materials[0] = material1;
    materials[1] = material2;
}

bool MixtureMaterial::TestAlpha(const Point2& uv) const
{
    return materials[0]->TestAlpha(uv) || materials[1]->TestAlpha(uv);
}

const SpectrumTexture* MixtureMaterial::GetNormalMap() const
{
    assert(false);
    return nullptr;
}

Spectrum MixtureMaterial::Le(const Intersection& isect, const Vec3& wo) const
{
    assert(false);
    return Spectrum::black;
}

bool MixtureMaterial::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    assert(false);
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
    return false;
}

} // namespace bulbit
