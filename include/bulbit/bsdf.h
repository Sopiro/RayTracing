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

    BSDF(Vec3 n, Vec3 t, BxDF* bxdf)
        : bxdf{ bxdf }
        , frame{ Frame::FromXZ(Normalize(t), n) }
    {
    }

    Spectrum f(Vec3 wo, Vec3 wi) const
    {
        wi = ToLocal(wi);
        wo = ToLocal(wo);

        if (wo.z == 0)
        {
            return Spectrum::black;
        }

        return bxdf->f(wo, wi);
    }

    bool Sample_f(
        BSDFSample* sample, Vec3 wo, Float u0, Point2 u12, BxDF_SamplingFlags sampleFlags = BxDF_SamplingFlags::All) const
    {
        assert(sample != nullptr);

        Vec3 wo = ToLocal(wo);
        if (wo.z == 0 || !(Flags() & sampleFlags))
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

        sample->wi = FromLocal(sample->wi);

        return sample;
    }

    Float PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags sampleFlags = BxDF_SamplingFlags::All) const
    {
        wo = ToLocal(wo);
        wi = ToLocal(wi);

        if (wo.z == 0)
        {
            return 0;
        }

        return bxdf->PDF(wo, wi, sampleFlags);
    }

    BxDF_Flags Flags() const
    {
        return bxdf->Flags();
    }

    Vec3 ToLocal(const Vec3& v) const
    {
        return frame.ToLocal(v);
    }

    Vec3 FromLocal(const Vec3& v) const
    {
        return frame.FromLocal(v);
    }

private:
    Frame frame;
    BxDF* bxdf;
};

} // namespace bulbit
