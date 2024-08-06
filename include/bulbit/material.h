#pragma once

#include "bsdf.h"
#include "bssrdf.h"
#include "intersectable.h"
#include "texture.h"

namespace bulbit
{

struct Intersection;

class Material
{
public:
    enum class Type
    {
        normal,
        subsurface,
        light_source,
        mixture,
    };

    Material(Type type)
        : type{ type }
    {
    }

    virtual ~Material() = default;

    virtual bool TestAlpha(const Point2& uv) const = 0;
    virtual const SpectrumTexture* GetNormalMap() const = 0;

    virtual Spectrum Le(const Intersection& isect, const Vec3& wo) const = 0;
    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const = 0;
    virtual bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
    {
        return false;
    }

    Type GetType() const
    {
        return type;
    }

private:
    Type type;
};

} // namespace bulbit