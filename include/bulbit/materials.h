#pragma once

#include "material.h"

namespace bulbit
{

class DiffuseMaterial : public Material
{
public:
    DiffuseMaterial(const Spectrum& albedo, const SpectrumTexture* normalmap = nullptr);
    DiffuseMaterial(const SpectrumTexture* albedo, const SpectrumTexture* normalmap = nullptr);

    bool TestAlpha(const Point2& uv) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

public:
    const SpectrumTexture* normalmap;
    const SpectrumTexture* albedo;
};

class MirrorMaterial : public Material
{
public:
    MirrorMaterial(const Spectrum& reflectance, const SpectrumTexture* normalmap = nullptr);
    MirrorMaterial(const SpectrumTexture* reflectance, const SpectrumTexture* normalmap = nullptr);

    bool TestAlpha(const Point2& uv) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

public:
    const SpectrumTexture* normalmap;
    const SpectrumTexture* reflectance;
};

class DielectricMaterial : public Material
{
public:
    DielectricMaterial(Float eta);
    DielectricMaterial(Float eta, Float roughness);
    DielectricMaterial(Float eta, const FloatTexture* roughness, const SpectrumTexture* normalmap = nullptr);
    DielectricMaterial(
        Float eta, const FloatTexture* u_roughness, const FloatTexture* v_roughness, const SpectrumTexture* normalmap = nullptr
    );

    bool TestAlpha(const Point2& uv) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

private:
    const SpectrumTexture* normalmap;
    const FloatTexture* u_roughness;
    const FloatTexture* v_roughness;

    Float eta;
};

class ThinDielectricMaterial : public Material
{
public:
    ThinDielectricMaterial(Float eta);

    bool TestAlpha(const Point2& uv) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

private:
    Float eta;
};

class ConductorMaterial : public Material
{
public:
    ConductorMaterial(const SpectrumTexture* eta, const SpectrumTexture* k, const FloatTexture* roughness);
    ConductorMaterial(
        const SpectrumTexture* eta,
        const SpectrumTexture* k,
        const FloatTexture* u_roughness,
        const FloatTexture* v_roughness,
        const SpectrumTexture* normalmap = nullptr
    );
    ConductorMaterial(const SpectrumTexture* reflectance, const FloatTexture* roughness);
    ConductorMaterial(
        const SpectrumTexture* reflectance,
        const FloatTexture* u_roughness,
        const FloatTexture* v_roughness,
        const SpectrumTexture* normalmap = nullptr
    );

    bool TestAlpha(const Point2& uv) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

private:
    const SpectrumTexture* normalmap;
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

    bool TestAlpha(const Point2& uv) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

private:
    const SpectrumTexture* normalmap;
    const FloatTexture* u_roughness;
    const FloatTexture* v_roughness;

    const SpectrumTexture* basecolor;
    const FloatTexture* metallic;
    const SpectrumTexture* emissive;
};

class DiffuseLightMaterial : public Material
{
public:
    DiffuseLightMaterial(const Spectrum& color, bool two_sided = false);
    DiffuseLightMaterial(const SpectrumTexture* emission, bool two_sided = false);

    bool TestAlpha(const Point2& uv) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

    bool two_sided;

private:
    const SpectrumTexture* emission;
};

class MixtureMaterial : public Material
{
public:
    MixtureMaterial(const Material* material1, const Material* material2, Float mix);
    MixtureMaterial(const Material* material1, const Material* material2, const FloatTexture* mix);

    bool TestAlpha(const Point2& uv) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

    const Material* ChooseMaterial(const Intersection& isect, const Vec3& wo) const;

private:
    const Material* materials[2];
    const FloatTexture* mixture_amount;
};

class SubsurfaceMaterialDiffusion : public Material
{
public:
    SubsurfaceMaterialDiffusion(
        const Spectrum& reflectance, const Spectrum& mfp, Float eta, Float roughness, const SpectrumTexture* normalmap = nullptr
    );
    SubsurfaceMaterialDiffusion(
        const SpectrumTexture* reflectance,
        const Spectrum& mfp,
        Float eta,
        Float roughness,
        const SpectrumTexture* normalmap = nullptr
    );
    SubsurfaceMaterialDiffusion(
        const SpectrumTexture* reflectance,
        const Spectrum& mfp,
        Float eta,
        const FloatTexture* roughness,
        const SpectrumTexture* normalmap = nullptr
    );
    SubsurfaceMaterialDiffusion(
        const SpectrumTexture* reflectance,
        const Spectrum& mfp,
        Float eta,
        const FloatTexture* u_roughness,
        const FloatTexture* v_roughness,
        const SpectrumTexture* normalmap = nullptr
    );

    bool TestAlpha(const Point2& uv) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

private:
    const SpectrumTexture* normalmap;
    const FloatTexture* u_roughness;
    const FloatTexture* v_roughness;

    const SpectrumTexture* reflectance;
    Spectrum l; // Mean free path
    Float eta;
};

class SubsurfaceMaterialRandomWalk : public Material
{
public:
    SubsurfaceMaterialRandomWalk(
        const Spectrum& reflectance, const Spectrum& mfp, Float eta, Float roughness, const SpectrumTexture* normalmap = nullptr
    );
    SubsurfaceMaterialRandomWalk(
        const SpectrumTexture* reflectance,
        const Spectrum& mfp,
        Float eta,
        Float roughness,
        const SpectrumTexture* normalmap = nullptr
    );
    SubsurfaceMaterialRandomWalk(
        const SpectrumTexture* reflectance,
        const Spectrum& mfp,
        Float eta,
        const FloatTexture* roughness,
        const SpectrumTexture* normalmap = nullptr
    );
    SubsurfaceMaterialRandomWalk(
        const SpectrumTexture* reflectance,
        const Spectrum& mfp,
        Float eta,
        const FloatTexture* u_roughness,
        const FloatTexture* v_roughness,
        const SpectrumTexture* normalmap = nullptr
    );

    bool TestAlpha(const Point2& uv) const;
    const SpectrumTexture* GetNormalMap() const;

    Spectrum Le(const Intersection& isect, const Vec3& wo) const;
    bool GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;
    bool GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const;

private:
    const SpectrumTexture* normalmap;
    const FloatTexture* u_roughness;
    const FloatTexture* v_roughness;

    const SpectrumTexture* reflectance;
    Spectrum l; // Mean free path
    Float eta;
};

inline bool Material::TestAlpha(const Point2& uv) const
{
    return Dispatch([&](auto mat) { return mat->TestAlpha(uv); });
}

inline const SpectrumTexture* Material::GetNormalMap() const
{
    return Dispatch([](auto mat) { return mat->GetNormalMap(); });
}

inline Spectrum Material::Le(const Intersection& isect, const Vec3& wo) const
{
    return Dispatch([&](auto mat) { return mat->Le(isect, wo); });
}

inline bool Material::GetBSDF(BSDF* bsdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    return Dispatch([&](auto mat) { return mat->GetBSDF(bsdf, isect, wo, alloc); });
}

inline bool Material::GetBSSRDF(BSSRDF** bssrdf, const Intersection& isect, const Vec3& wo, Allocator& alloc) const
{
    return Dispatch([&](auto mat) { return mat->GetBSSRDF(bssrdf, isect, wo, alloc); });
}

} // namespace bulbit
