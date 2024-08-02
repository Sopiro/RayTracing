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
    virtual bool Sample_S(BSSRDFSample* bssrdf_sample, const Intersectable* accel, Float u1, const Point2& u2, Allocator& alloc)
        const = 0;

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

    virtual bool Sample_S(BSSRDFSample* bssrdf_sample, const Intersectable* accel, Float u1, const Point2& u2, Allocator& alloc)
        const override;

    bool Sample_Sp(BSSRDFSample* bssrdf_sample, const Intersectable* accel, Float u1, const Point2& u2, Allocator& alloc) const;
    Float PDF_Sp(const Intersection& pi) const;

    virtual Spectrum Sr(Float d) const = 0;
    virtual Float Sample_Sr(int32 wavelength, Float u) const = 0;
    virtual Spectrum PDF_Sr(int32 wavelength, Float r) const = 0;

private:
    NormalizedFresnelBxDF sw;
};

} // namespace bulbit
