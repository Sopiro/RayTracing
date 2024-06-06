#pragma once

#include "bsdf.h"
#include "intersectable.h"
#include "texture.h"

namespace bulbit
{

struct Intersection;

class Material
{
public:
    virtual ~Material() = default;

    virtual bool TestAlpha(const Point2& uv) const = 0;
    virtual Spectrum Le(const Intersection& isect, const Vec3& wo) const = 0;
    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const = 0;
};

class DiffuseMaterial : public Material
{
public:
    DiffuseMaterial(const Spectrum& color);
    DiffuseMaterial(const SpectrumTexture* albedo);

    virtual bool TestAlpha(const Point2& uv) const override;
    virtual Spectrum Le(const Intersection& isect, const Vec3& wo) const override;
    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const override;

public:
    const SpectrumTexture* albedo;
};

class UnrealMaterial : public Material
{
public:
    UnrealMaterial(const SpectrumTexture* basecolor,
                   const FloatTexture* metallic,
                   const FloatTexture* roughness,
                   const SpectrumTexture* emissive = nullptr,
                   const SpectrumTexture* normalmap = nullptr);

    virtual bool TestAlpha(const Point2& uv) const override;
    virtual Spectrum Le(const Intersection& isect, const Vec3& wo) const override;
    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const override;

private:
    void NormalMapping(Vec3* normal, Vec3* tangent, const Intersection& isect) const;

    const SpectrumTexture* basecolor;
    const FloatTexture* metallic;
    const FloatTexture* roughness;
    const SpectrumTexture* emissive;
    const SpectrumTexture* normalmap;
};

class DiffuseLightMaterial : public Material
{
public:
    DiffuseLightMaterial(const Spectrum& color, bool two_sided = false);
    DiffuseLightMaterial(const SpectrumTexture* emission, bool two_sided = false);

    virtual bool TestAlpha(const Point2& uv) const override;
    virtual Spectrum Le(const Intersection& isect, const Vec3& wo) const override;
    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const override;

    const SpectrumTexture* emission;
    bool two_sided;
};

} // namespace bulbit