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

    // clang-format off
    virtual bool IsLightSource() const { return false; }
    virtual bool TestAlpha(const Point2& uv) const { return true; }
    virtual Spectrum Le(const Intersection& isect, const Vec3& wi) const { return Spectrum::black; }
    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const = 0;
    // clang-format off
};

class DiffuseMaterial : public Material
{
public:
    DiffuseMaterial(const Spectrum& color);
    DiffuseMaterial(const SpectrumTexture* albedo);

    virtual bool TestAlpha(const Point2& uv) const override;
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
                      const SpectrumTexture* emissive = ConstantColorTexture::Create(Float(0.0)),
                      const SpectrumTexture* normalmap = ConstantColorTexture::Create(Float(0.5), Float(0.5), Float(1.0)));

    virtual bool TestAlpha(const Point2& uv) const override;
    virtual Spectrum Le(const Intersection& isect, const Vec3& wi) const override;
    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const override;

private:
    void NormalMapping(Vec3* normal, Vec3* tangent, const Intersection& isect) const;

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
    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const override;

    const SpectrumTexture* emission;
    bool two_sided;
};

} // namespace bulbit