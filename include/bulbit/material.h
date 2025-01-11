#pragma once

#include "bsdf.h"
#include "bssrdf.h"
#include "dynamic_dispatcher.h"
#include "intersectable.h"
#include "texture.h"

namespace bulbit
{

struct Intersection;

using Materials = TypePack<
    class DiffuseMaterial,
    class MirrorMaterial,
    class DielectricMaterial,
    class ThinDielectricMaterial,
    class ConductorMaterial,
    class UnrealMaterial,
    class DiffuseLightMaterial,
    class MixtureMaterial,
    class SubsurfaceMaterialDiffusion,
    class SubsurfaceMaterialRandomWalk>;

class Material : public DynamicDispatcher<Materials>
{
protected:
    Material(int8 type_index)
        : DynamicDispatcher{ type_index }
    {
    }

public:
    bool TestAlpha(const Point2& uv) const;
    const SpectrumTexture* GetNormalMap() const;
    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
};

} // namespace bulbit