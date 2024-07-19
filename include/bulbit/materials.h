#pragma once

#include "material.h"

namespace bulbit
{

class DiffuseMaterial : public Material
{
public:
    DiffuseMaterial(const Spectrum& albedo);
    DiffuseMaterial(const SpectrumTexture* albedo);

    virtual bool TestAlpha(const Point2& uv) const override;
    virtual const SpectrumTexture* GetNormalMap() const override;

    virtual Spectrum Le(const Intersection& isect, const Vec3& wo) const override;
    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const override;

public:
    const SpectrumTexture* albedo;
};

class MirrorMaterial : public Material
{
public:
    MirrorMaterial(const Spectrum& reflectance);
    MirrorMaterial(const SpectrumTexture* reflectance);

    virtual bool TestAlpha(const Point2& uv) const override;
    virtual const SpectrumTexture* GetNormalMap() const override;

    virtual Spectrum Le(const Intersection& isect, const Vec3& wo) const override;
    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const override;

public:
    const SpectrumTexture* reflectance;
};

class DielectricMaterial : public Material
{
public:
    DielectricMaterial(Float eta);
    DielectricMaterial(Float eta, Float roughness);
    DielectricMaterial(Float eta, const FloatTexture* roughness);
    DielectricMaterial(Float eta, const FloatTexture* u_roughness, const FloatTexture* v_roughness);

    virtual bool TestAlpha(const Point2& uv) const override;
    virtual const SpectrumTexture* GetNormalMap() const override;

    virtual Spectrum Le(const Intersection& isect, const Vec3& wo) const override;
    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const override;

private:
    const FloatTexture* u_roughness;
    const FloatTexture* v_roughness;
    Float eta;
};

class ThinDielectricMaterial : public Material
{
public:
    ThinDielectricMaterial(Float eta);

    virtual bool TestAlpha(const Point2& uv) const override;
    virtual const SpectrumTexture* GetNormalMap() const override;

    virtual Spectrum Le(const Intersection& isect, const Vec3& wo) const override;
    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const override;

private:
    Float eta;
};

class ConductorMaterial : public Material
{
public:
    ConductorMaterial(const SpectrumTexture* eta, const SpectrumTexture* k, const FloatTexture* roughness);
    ConductorMaterial(
        const SpectrumTexture* eta, const SpectrumTexture* k, const FloatTexture* u_roughness, const FloatTexture* v_roughness
    );
    ConductorMaterial(const SpectrumTexture* reflectance, const FloatTexture* roughness);
    ConductorMaterial(const SpectrumTexture* reflectance, const FloatTexture* u_roughness, const FloatTexture* v_roughness);

    virtual bool TestAlpha(const Point2& uv) const override;
    virtual const SpectrumTexture* GetNormalMap() const override;

    virtual Spectrum Le(const Intersection& isect, const Vec3& wo) const override;
    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const override;

private:
    const FloatTexture* u_roughness;
    const FloatTexture* v_roughness;
    const SpectrumTexture* eta;
    const SpectrumTexture* k;
};

class UnrealMaterial : public Material
{
public:
    UnrealMaterial(
        const SpectrumTexture* basecolor,
        const FloatTexture* metallic,
        const FloatTexture* roughness,
        const SpectrumTexture* emissive = nullptr,
        const SpectrumTexture* normalmap = nullptr
    );
    UnrealMaterial(
        const SpectrumTexture* basecolor,
        const FloatTexture* metallic,
        const FloatTexture* u_roughness,
        const FloatTexture* v_roughness,
        const SpectrumTexture* emissive = nullptr,
        const SpectrumTexture* normalmap = nullptr
    );

    virtual bool TestAlpha(const Point2& uv) const override;
    virtual const SpectrumTexture* GetNormalMap() const override;

    virtual Spectrum Le(const Intersection& isect, const Vec3& wo) const override;
    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const override;

private:
    const SpectrumTexture* basecolor;
    const FloatTexture* metallic;
    const FloatTexture* u_roughness;
    const FloatTexture* v_roughness;
    const SpectrumTexture* emissive;
    const SpectrumTexture* normalmap;
};

class DiffuseLightMaterial : public Material
{
public:
    DiffuseLightMaterial(const Spectrum& color, bool two_sided = false);
    DiffuseLightMaterial(const SpectrumTexture* emission, bool two_sided = false);

    virtual bool TestAlpha(const Point2& uv) const override;
    virtual const SpectrumTexture* GetNormalMap() const override;

    virtual Spectrum Le(const Intersection& isect, const Vec3& wo) const override;
    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const override;

    bool two_sided;

private:
    const SpectrumTexture* emission;
};

class MixtureMaterial : public Material
{
public:
    MixtureMaterial(const Material* material1, const Material* material2, Float mix);
    MixtureMaterial(const Material* material1, const Material* material2, const FloatTexture* mix);

    virtual bool TestAlpha(const Point2& uv) const override;
    virtual const SpectrumTexture* GetNormalMap() const override;

    virtual Spectrum Le(const Intersection& isect, const Vec3& wo) const override;
    virtual bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const override;

    const Material* ChooseMaterial(const Intersection& isect, const Vec3& wo);

private:
    const Material* materials[2];
    const FloatTexture* mixture_amount;
};

} // namespace bulbit
