#pragma once

#include "spectrum.h"

#include <span>

namespace bulbit
{

enum BxDF_Flags
{
    Unset = 0,
    Reflection = 1 << 0,
    Transmission = 1 << 1,
    Diffuse = 1 << 2,
    Glossy = 1 << 3,
    Specular = 1 << 4,
    DiffuseReflection = Diffuse | Reflection,
    DiffuseTransmission = Diffuse | Transmission,
    GlossyReflection = Glossy | Reflection,
    GlossyTransmission = Glossy | Transmission,
    SpecularReflection = Specular | Reflection,
    SpecularTransmission = Specular | Transmission,
    All = Diffuse | Glossy | Specular | Reflection | Transmission,
};

enum class BxDF_SamplingFlags
{
    Unset = 0,
    Reflection = 1 << 0,
    Transmission = 1 << 1,
    All = Reflection | Transmission
};

inline BxDF_Flags operator|(BxDF_Flags a, BxDF_Flags b)
{
    return BxDF_Flags((int32)a | (int32)b);
}

inline int32 operator&(BxDF_Flags a, BxDF_Flags b)
{
    return ((int32)a & (int32)b);
}

inline int32 operator&(BxDF_Flags a, BxDF_SamplingFlags b)
{
    return ((int32)a & (int32)b);
}

inline BxDF_Flags& operator|=(BxDF_Flags& a, BxDF_Flags b)
{
    (int32&)a |= int32(b);
    return a;
}

bool IsReflective(BxDF_Flags f)
{
    return f & BxDF_Flags::Reflection;
}

bool IsTransmissive(BxDF_Flags f)
{
    return f & BxDF_Flags::Transmission;
}

bool IsDiffuse(BxDF_Flags f)
{
    return f & BxDF_Flags::Diffuse;
}

bool IsGlossy(BxDF_Flags f)
{
    return f & BxDF_Flags::Glossy;
}

bool IsSpecular(BxDF_Flags f)
{
    return f & BxDF_Flags::Specular;
}

bool IsNonSpecular(BxDF_Flags f)
{
    return f & (BxDF_Flags::Diffuse | BxDF_Flags::Glossy);
}

struct BSDFSample
{
    BSDFSample() = default;

    BSDFSample(Spectrum f, Vec3 wi, Float pdf, BxDF_Flags flags, Float eta = 1, bool pdf_is_proportional = false)
        : f{ f }
        , wi{ wi }
        , pdf{ pdf }
        , flags{ flags }
        , eta{ eta }
        , pdf_is_proportional{ pdf_is_proportional }
    {
    }

    bool IsReflection() const
    {
        return bulbit::IsReflective(flags);
    }

    bool IsTransmission() const
    {
        return bulbit::IsTransmissive(flags);
    }

    bool IsDiffuse() const
    {
        return bulbit::IsDiffuse(flags);
    }

    bool IsGlossy() const
    {
        return bulbit::IsGlossy(flags);
    }

    bool IsSpecular() const
    {
        return bulbit::IsSpecular(flags);
    }

    Spectrum f;
    Vec3 wi;
    Float pdf = 0;
    BxDF_Flags flags;
    Float eta = 1;
    bool pdf_is_proportional = false;
};

class BxDF
{
public:
    virtual BxDF_Flags Flags() const = 0;

    virtual Spectrum f(const Vec3& wo, const Vec3& wi) const = 0;

    virtual bool Sample_f(
        BSDFSample* sample, Vec3 wo, Float u0, Point2 u1, BxDF_SamplingFlags sampleFlags = BxDF_SamplingFlags::All) const = 0;

    virtual Float PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags sampleFlags = BxDF_SamplingFlags::All) const = 0;

    // Compute Hemispherical reflectance (albedo)
    Spectrum rho(Vec3 wo, std::span<const Float> uc, std::span<const Point2> u2) const;

    // Compute Hemispherical-Hemispherical reflectance
    Spectrum rho(std::span<const Point2> u1, std::span<const Float> uc, std::span<const Point2> u2) const;
};

} // namespace bulbit
