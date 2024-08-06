#pragma once

#include "bxdf.h"
#include "microfacet.h"
#include "scattering.h"

namespace bulbit
{

class LambertianBxDF : public BxDF
{
public:
    LambertianBxDF(const Spectrum& reflectance)
        : r{ reflectance }
    {
    }

    virtual BxDF_Flags Flags() const override
    {
        return !r.IsBlack() ? BxDF_Flags::DiffuseReflection : BxDF_Flags::Unset;
    }

    virtual Spectrum f(const Vec3& wo, const Vec3& wi) const override;
    virtual Float PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags flags = BxDF_SamplingFlags::All) const override;

    virtual bool Sample_f(BSDFSample* sample, Vec3 wo, Float u0, Point2 u12, BxDF_SamplingFlags flags = BxDF_SamplingFlags::All)
        const override;

private:
    // Reflectance [0, 1]
    Spectrum r;
};

class SpecularReflectionBxDF : public BxDF
{
public:
    SpecularReflectionBxDF(const Spectrum& reflectance)
        : r{ reflectance }
    {
    }

    virtual BxDF_Flags Flags() const override
    {
        return !r.IsBlack() ? BxDF_Flags::SpecularReflection : BxDF_Flags::Unset;
    }

    virtual Spectrum f(const Vec3& wo, const Vec3& wi) const override;
    virtual Float PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags flags = BxDF_SamplingFlags::All) const override;

    virtual bool Sample_f(BSDFSample* sample, Vec3 wo, Float u0, Point2 u12, BxDF_SamplingFlags flags = BxDF_SamplingFlags::All)
        const override;

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
    virtual Float PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags flags = BxDF_SamplingFlags::All) const override;

    virtual bool Sample_f(BSDFSample* sample, Vec3 wo, Float u0, Point2 u12, BxDF_SamplingFlags flags = BxDF_SamplingFlags::All)
        const override;

    virtual void Regularize() override;

private:
    Float eta;
    TrowbridgeReitzDistribution mf;
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
    virtual Float PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags flags = BxDF_SamplingFlags::All) const override;

    virtual bool Sample_f(BSDFSample* sample, Vec3 wo, Float u0, Point2 u12, BxDF_SamplingFlags flags = BxDF_SamplingFlags::All)
        const override;

    virtual void Regularize() override;

private:
    TrowbridgeReitzDistribution mf;
    Spectrum eta, k;
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
    virtual Float PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags flags = BxDF_SamplingFlags::All) const override;

    virtual bool Sample_f(BSDFSample* sample, Vec3 wo, Float u0, Point2 u12, BxDF_SamplingFlags flags = BxDF_SamplingFlags::All)
        const override;

private:
    Float eta;
};

class UnrealBxDF : public BxDF
{
public:
    UnrealBxDF(Spectrum basecolor, Float metallic, TrowbridgeReitzDistribution mf, Float t)
        : basecolor{ basecolor }
        , metallic{ metallic }
        , mf{ mf }
        , t{ t }
    {
    }

    virtual BxDF_Flags Flags() const override
    {
        return BxDF_Flags::Diffuse | BxDF_Flags::Glossy | BxDF_Flags::Reflection;
    }

    virtual Spectrum f(const Vec3& wo, const Vec3& wi) const override;
    virtual Float PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags flags = BxDF_SamplingFlags::All) const override;

    virtual bool Sample_f(BSDFSample* sample, Vec3 wo, Float u0, Point2 u12, BxDF_SamplingFlags flags = BxDF_SamplingFlags::All)
        const override;

    virtual void Regularize() override;

    // Functions for microfacet BRDF

    // Default reflectance of dielectrics
    static const inline Vec3 default_reflectance = Vec3(0.04f);
    static const inline Float min_alpha = 0.003f;

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

private:
    Spectrum basecolor;
    Float metallic;
    TrowbridgeReitzDistribution mf;
    Float t;
};

class NormalizedFresnelBxDF : public BxDF
{
public:
    NormalizedFresnelBxDF(Float eta)
        : eta{ eta }
    {
    }

    virtual BxDF_Flags Flags() const override
    {
        return BxDF_Flags(BxDF_Flags::Diffuse | BxDF_Flags::Reflection);
    }

    virtual Spectrum f(const Vec3& wo, const Vec3& wi) const override;
    virtual Float PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags flags = BxDF_SamplingFlags::All) const override;

    virtual bool Sample_f(BSDFSample* sample, Vec3 wo, Float u0, Point2 u12, BxDF_SamplingFlags flags = BxDF_SamplingFlags::All)
        const override;

    virtual void Regularize() override;

private:
    Float eta;
};

constexpr size_t max_bxdf_size =
    std::max({ sizeof(LambertianBxDF), sizeof(SpecularReflectionBxDF), sizeof(DielectricBxDF), sizeof(ConductorBxDF),
               sizeof(ThinDielectricBxDF), sizeof(UnrealBxDF), sizeof(NormalizedFresnelBxDF) });

} // namespace bulbit
