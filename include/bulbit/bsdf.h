#pragma once

#include "bxdf.h"
#include "frame.h"

namespace bulbit
{

// BxDF bound with shading frame
class BSDF
{
public:
    BSDF() = default;
    BSDF(Vec3 n, BxDF* bxdf);
    BSDF(Vec3 n, Vec3 t, BxDF* bxdf);

    Spectrum f(Vec3 wo, Vec3 wi) const;

    bool Sample_f(
        BSDFSample* sample, Vec3 wo, Float u0, Point2 u12, BxDF_SamplingFlags sample_flags = BxDF_SamplingFlags::All) const;

    Float PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags sample_flags = BxDF_SamplingFlags::All) const;

    Spectrum rho(Vec3 wo, std::span<const Float> uc, std::span<const Point2> u) const;
    Spectrum rho(std::span<const Point2> u1, std::span<const Float> uc, std::span<const Point2> u2) const;

    Vec3 WorldToLocal(const Vec3& v) const;
    Vec3 LocalToWorld(const Vec3& v) const;

    void Regularize();

    BxDF_Flags Flags() const;

private:
    Frame frame;
    BxDF* bxdf;
};

inline BSDF::BSDF(Vec3 n, BxDF* bxdf)
    : bxdf{ bxdf }
    , frame{ Frame::FromZ(n) }
{
}

inline BSDF::BSDF(Vec3 n, Vec3 t, BxDF* bxdf)
    : bxdf{ bxdf }
    , frame{ Frame::FromXZ(t, n) }
{
}

inline Spectrum BSDF::f(Vec3 wo, Vec3 wi) const
{
    wi = WorldToLocal(wi);
    wo = WorldToLocal(wo);

    if (wo.z == 0)
    {
        return Spectrum::black;
    }

    return bxdf->f(wo, wi);
}

inline bool BSDF::Sample_f(BSDFSample* sample, Vec3 wo, Float u0, Point2 u12, BxDF_SamplingFlags sampleFlags) const
{
    assert(sample != nullptr);

    wo = WorldToLocal(wo);
    if (wo.z == 0 || !(bxdf->Flags() & sampleFlags))
    {
        return false;
    }

    if (!bxdf->Sample_f(sample, wo, u0, u12, sampleFlags))
    {
        return false;
    }

    if (sample->f.IsBlack() || sample->pdf == 0 || sample->wi.z == 0)
    {
        return false;
    }

    sample->wi = LocalToWorld(sample->wi);
    return true;
}

inline Float BSDF::PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags sampleFlags) const
{
    wo = WorldToLocal(wo);
    wi = WorldToLocal(wi);

    if (wo.z == 0)
    {
        return 0;
    }

    return bxdf->PDF(wo, wi, sampleFlags);
}

inline Spectrum BSDF::rho(std::span<const Point2> u1, std::span<const Float> uc, std::span<const Point2> u2) const
{
    return bxdf->rho(u1, uc, u2);
}

inline Spectrum BSDF::rho(Vec3 wo, std::span<const Float> uc, std::span<const Point2> u) const
{
    wo = WorldToLocal(wo);
    return bxdf->rho(wo, uc, u);
}

inline Vec3 BSDF::WorldToLocal(const Vec3& v) const
{
    return frame.ToLocal(v);
}

inline Vec3 BSDF::LocalToWorld(const Vec3& v) const
{
    return frame.FromLocal(v);
}

inline BxDF_Flags BSDF::Flags() const
{
    return bxdf->Flags();
}

inline void BSDF::Regularize()
{
    bxdf->Regularize();
}

} // namespace bulbit
