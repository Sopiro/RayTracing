#pragma once

#include "bsdf.h"
#include "bxdfs.h"
#include "intersectable.h"
#include "scattering.h"

namespace bulbit
{

struct BSSRDFSample
{
    Intersection pi;
    Spectrum Sp, pdf;
    Float p;
    BSDF Sw;
};

class BSSRDF
{
public:
    BSSRDF(const Intersection& po, const Vec3& wo, Float eta)
        : po{ po }
        , wo{ wo }
        , eta{ eta }
    {
    }

    virtual Spectrum S(const Intersection& pi, const Vec3& wi) const = 0;
    virtual bool Sample_S(
        BSSRDFSample* bssrdf_sample, const Intersectable* accel, int32 wavelength, Float u1, const Point2& u2, Allocator& alloc
    ) = 0;

protected:
    const Intersection& po;
    const Vec3& wo;
    Float eta;
};

class SeparableBSSRDF : public BSSRDF
{
public:
    SeparableBSSRDF(const Intersection& po, const Vec3& wo, Float eta)
        : BSSRDF(po, wo, eta)
        , sw(eta)
    {
    }

    virtual Spectrum S(const Intersection& pi, const Vec3& wi) const override;
    Spectrum Sw(const Intersection& pi, const Vec3& wi) const;
    Spectrum Sp(const Intersection& pi) const;

    virtual bool Sample_S(
        BSSRDFSample* bssrdf_sample, const Intersectable* accel, int32 wavelength, Float u1, const Point2& u2, Allocator& alloc
    ) override;

    Spectrum PDF_Sp(const Intersection& pi) const;

    virtual Float MaxSr() const = 0;
    virtual Spectrum Sr(Float d) const = 0;
    virtual Float Sample_Sr(int32 wavelength, Float u) const = 0;
    virtual Spectrum PDF_Sr(Float r) const = 0;

private:
    static inline Float axis_sampling_probabilities[3] = { 0.25f, 0.25f, 0.5f };
    NormalizedFresnelBxDF sw;
};

} // namespace bulbit
