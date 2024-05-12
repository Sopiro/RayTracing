#include "bulbit/bxdfs.h"
#include "bulbit/frame.h"
#include "bulbit/sampling.h"

namespace bulbit
{

Spectrum DiffuseBxDF::f(const Vec3& wo, const Vec3& wi) const
{
    if (!SameHemisphere(wo, wi))
    {
        return Spectrum(0);
    }

    return r * inv_pi;
}

bool DiffuseBxDF::Sample_f(BSDFSample* sample, Vec3 wo, Float u0, Point2 u12, BxDF_SamplingFlags sampleFlags) const
{
    if (!(sampleFlags & BxDF_SamplingFlags::Reflection))
    {
        return false;
    }

    Vec3 wi = CosineSampleHemisphere(u12);
    if (wi.z < 0)
    {
        wi.z = -wi.z;
    }
    Float pdf = AbsCosTheta(wi);

    *sample = BSDFSample(r, wi, pdf, BxDF_Flags::DiffuseReflection);
    return true;
}

Float DiffuseBxDF::PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags sampleFlags) const
{
    if (!(sampleFlags & BxDF_SamplingFlags::Reflection) || !SameHemisphere(wo, wi))
    {
        return 0;
    }

    return CosineSampleHemispherePDF(AbsCosTheta(wi));
}

} // namespace bulbit
