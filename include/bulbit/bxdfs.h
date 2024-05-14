#pragma once

#include "bxdf.h"

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

    virtual bool Sample_f(BSDFSample* sample,
                          Vec3 wo,
                          Float u0,
                          Point2 u12,
                          BxDF_SamplingFlags sampleFlags = BxDF_SamplingFlags::All) const override;

    virtual Float PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags sampleFlags = BxDF_SamplingFlags::All) const override;

private:
    // Reflectance [0, 1]
    Spectrum r;
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

    virtual bool Sample_f(BSDFSample* sample,
                          Vec3 wo,
                          Float u0,
                          Point2 u12,
                          BxDF_SamplingFlags sample_flags = BxDF_SamplingFlags::All) const override;

    virtual Float PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags sample_flags = BxDF_SamplingFlags::All) const override;

private:
    Spectrum basecolor;
    Float metallic, alpha;
    Float t;
};

constexpr size_t max_bxdf_size = std::max({ sizeof(DiffuseBxDF), sizeof(UnrealBxDF) });

} // namespace bulbit
