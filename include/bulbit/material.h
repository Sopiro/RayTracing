#pragma once

#include "bsdf.h"
#include "intersectable.h"
#include "texture.h"

namespace bulbit
{

struct Intersection;

struct Interaction
{
    BSDF bsdf;

private:
    friend class DiffuseMaterial;
    friend class Microfacet;

    int8 mem[128];
};

class Material
{
public:
    virtual ~Material() = default;

    virtual bool IsLightSource() const
    {
        return false;
    }

    virtual Spectrum Emit(const Intersection& is, const Vec3& wi) const
    {
        return RGBSpectrum::black;
    }

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const = 0;

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

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const override;
    virtual bool TestAlpha(const Point2& uv) const override;

public:
    const SpectrumTexture* albedo;
};

// Microfacet material model
// BRDF (Cook-Torrance specular + Lambertian diffuse)
// - GGX normal distribution function
// - Smith-GGX height-correlated visibility function
// - width Schlick Fresnel blend
class Microfacet : public Material
{
public:
    Microfacet(const SpectrumTexture* basecolor,
               const FloatTexture* metallic,
               const FloatTexture* roughness,
               const SpectrumTexture* emissive = ConstantColorTexture::Create(Float(0.0)),
               const SpectrumTexture* normalmap = ConstantColorTexture::Create(Float(0.5), Float(0.5), Float(1.0)));

    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const override;
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

    virtual Spectrum Emit(const Intersection& is, const Vec3& wi) const override;
    virtual bool Scatter(Interaction* out_ir, const Intersection& is, const Vec3& wi, const Point2& u) const override;

    const SpectrumTexture* emission;
    bool two_sided;
};

} // namespace bulbit