#pragma once

#include "bxdf.h"
#include "microfacet.h"

namespace bulbit
{

// Lambertian BRDF
class DiffuseBxDF : public BxDF
{
public:
    DiffuseBxDF(const Spectrum& reflectance)
        : r{ reflectance }
    {
    }

    virtual BxDF_Flags Flags() const override
    {
        return !r.IsBlack() ? BxDF_Flags::DiffuseReflection : BxDF_Flags::Unset;
    }

    virtual Spectrum f(const Vec3& wo, const Vec3& wi) const override;
    virtual Float PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags sampleFlags = BxDF_SamplingFlags::All) const override;

    virtual bool Sample_f(BSDFSample* sample,
                          Vec3 wo,
                          Float u0,
                          Point2 u12,
                          BxDF_SamplingFlags sampleFlags = BxDF_SamplingFlags::All) const override;

private:
    // Reflectance [0, 1]
    Spectrum r;
};

class DielectricBxDF : public BxDF
{
public:
    DielectricBxDF(Float eta, TrowbridgeReitzDistribution mf)
        : eta{ eta }
        , mf{ mf }
    {
    }

    virtual BxDF_Flags Flags() const override
    {
        BxDF_Flags flags = (eta == 1) ? BxDF_Flags::Transmission : (BxDF_Flags::Reflection | BxDF_Flags::Transmission);
        return flags | (mf.EffectivelySmooth() ? BxDF_Flags::Specular : BxDF_Flags::Glossy);
    }

    virtual Spectrum f(const Vec3& wo, const Vec3& wi) const override;
    virtual Float PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags sampleFlags = BxDF_SamplingFlags::All) const override;

    virtual bool Sample_f(BSDFSample* sample,
                          Vec3 wo,
                          Float u0,
                          Point2 u12,
                          BxDF_SamplingFlags sampleFlags = BxDF_SamplingFlags::All) const override;

private:
    Float eta;
    TrowbridgeReitzDistribution mf;
};

class ThinDielectricBxDF : public BxDF
{
public:
    ThinDielectricBxDF(Float eta)
        : eta{ eta }
    {
    }

    virtual BxDF_Flags Flags() const override
    {
        return BxDF_Flags::Specular | BxDF_Flags::Reflection | BxDF_Flags::Transmission;
    }

    virtual Spectrum f(const Vec3& wo, const Vec3& wi) const override;
    virtual Float PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags sampleFlags = BxDF_SamplingFlags::All) const override;

    virtual bool Sample_f(BSDFSample* sample,
                          Vec3 wo,
                          Float u0,
                          Point2 u12,
                          BxDF_SamplingFlags sampleFlags = BxDF_SamplingFlags::All) const override;

private:
    Float eta;
};

class ConductorBxDF : public BxDF
{
public:
    ConductorBxDF(Spectrum eta, Spectrum k, TrowbridgeReitzDistribution mf)
        : eta{ eta }
        , k{ k }
        , mf{ mf }
    {
    }

    virtual BxDF_Flags Flags() const override
    {
        return mf.EffectivelySmooth() ? BxDF_Flags::SpecularReflection : BxDF_Flags::GlossyReflection;
    }

    virtual Spectrum f(const Vec3& wo, const Vec3& wi) const override;
    virtual Float PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags sampleFlags = BxDF_SamplingFlags::All) const override;

    virtual bool Sample_f(BSDFSample* sample,
                          Vec3 wo,
                          Float u0,
                          Point2 u12,
                          BxDF_SamplingFlags sampleFlags = BxDF_SamplingFlags::All) const override;

private:
    TrowbridgeReitzDistribution mf;
    Spectrum eta, k;
};

class UnrealBxDF : public BxDF
{
public:
    UnrealBxDF(Spectrum basecolor, Float metallic, Float alpha, Float t)
        : basecolor{ basecolor }
        , metallic{ metallic }
        , alpha{ alpha }
        , t{ t }
    {
    }

    virtual BxDF_Flags Flags() const override
    {
        return BxDF_Flags::Diffuse | BxDF_Flags::Glossy | BxDF_Flags::Reflection;
    }

    virtual Spectrum f(const Vec3& wo, const Vec3& wi) const override;
    virtual Float PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags sample_flags = BxDF_SamplingFlags::All) const override;

    virtual bool Sample_f(BSDFSample* sample,
                          Vec3 wo,
                          Float u0,
                          Point2 u12,
                          BxDF_SamplingFlags sample_flags = BxDF_SamplingFlags::All) const override;

    virtual void Regularize() override;

    // Functions for microfacet BRDF

    // Default reflectance of dielectrics
    static const inline Vec3 default_reflectance = Vec3(0.04f);
    static const inline Float min_alpha = 0.002f;

    static Float RoughnessToAlpha(Float roughness)
    {
        return std::fmax(roughness * roughness, min_alpha);
    }

    static Spectrum F0(Spectrum basecolor, Float metallic)
    {
        return Lerp(default_reflectance, basecolor, metallic);
    }

    static Spectrum F_Schlick(Spectrum f0, Float cosine_theta)
    {
        return f0 + (/*f90*/ Spectrum(1) - f0) * std::pow(1 - cosine_theta, 5.0f);
    }

    // Trowbridge-Reitz distribution
    static Float D_GGX(Float NoH, Float alpha2)
    {
        Float b = (NoH * NoH * (alpha2 - 1) + 1);
        return alpha2 * inv_pi / (b * b + 1e-7f);
    }

    static Float G1_Smith(Float NoV, Float alpha2)
    {
        return 2 * NoV / (NoV + std::sqrt(alpha2 + (1 - alpha2) * NoV * NoV));
    }

    static Float G2_Smith_Correlated(Float NoV, Float NoL, Float alpha2)
    {
        Float g1 = NoV * std::sqrt(alpha2 + (1 - alpha2) * NoL * NoL);
        Float g2 = NoL * std::sqrt(alpha2 + (1 - alpha2) * NoV * NoV);
        return 2 * NoL * NoV / (g1 + g2);
    }

    static Float V_Smith_Correlated(Float NoV, Float NoL, Float alpha2)
    {
        Float g1 = NoV * std::sqrt(alpha2 + (1 - alpha2) * NoL * NoL);
        Float g2 = NoL * std::sqrt(alpha2 + (1 - alpha2) * NoV * NoV);
        return 0.5f / (g1 + g2);
    }

private:
    Spectrum basecolor;
    Float metallic, alpha;
    Float t;
};

constexpr size_t max_bxdf_size = std::max(
    { sizeof(DiffuseBxDF), sizeof(UnrealBxDF), sizeof(DielectricBxDF), sizeof(ThinDielectricBxDF), sizeof(ConductorBxDF) });

} // namespace bulbit
