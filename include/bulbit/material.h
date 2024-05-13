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

    virtual bool IsLightSource() const
    {
        return false;
    }

    virtual Spectrum Le(const Intersection& isect, const Vec3& wi) const
    {
        return Spectrum::black;
    }

    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wi, const Point2& u, Allocator& alloc) const = 0;

    virtual bool TestAlpha(const Point2& uv) const
    {
        return true;
    }
};

class DiffuseMaterial : public Material
{
public:
    DiffuseMaterial(const Spectrum& color);
    DiffuseMaterial(const SpectrumTexture* albedo);

    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wi, const Point2& u, Allocator& alloc) const override;
    virtual bool TestAlpha(const Point2& uv) const override;

public:
    const SpectrumTexture* albedo;
};

class UnrealishMaterial : public Material
{
public:
    UnrealishMaterial(const SpectrumTexture* basecolor,
                      const FloatTexture* metallic,
                      const FloatTexture* roughness,
                      const SpectrumTexture* emissive = ConstantColorTexture::Create(Float(0.0)),
                      const SpectrumTexture* normalmap = ConstantColorTexture::Create(Float(0.5), Float(0.5), Float(1.0)));

    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wi, const Point2& u, Allocator& alloc) const override;
    virtual bool TestAlpha(const Point2& uv) const override;

private:
    const SpectrumTexture* basecolor;
    const FloatTexture* metallic;
    const FloatTexture* roughness;
    const SpectrumTexture* emissive;
    const SpectrumTexture* normalmap;
};

class DiffuseLight : public Material
{
public:
    DiffuseLight(const Spectrum& color, bool two_sided = false);
    DiffuseLight(const SpectrumTexture* emission, bool two_sided = false);

    virtual bool IsLightSource() const override;

    virtual Spectrum Le(const Intersection& isect, const Vec3& wi) const override;
    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wi, const Point2& u, Allocator& alloc) const override;

    const SpectrumTexture* emission;
    bool two_sided;
};

} // namespace bulbit