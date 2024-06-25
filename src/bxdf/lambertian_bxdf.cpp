#include "bulbit/bxdfs.h"
#include "bulbit/frame.h"
#include "bulbit/sampling.h"

namespace bulbit
{

Spectrum LambertianBxDF::f(const Vec3& wo, const Vec3& wi) const
{
    if (!SameHemisphere(wo, wi))
    {
        return Spectrum::black;
    }

    return r * inv_pi;
}

Float LambertianBxDF::PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags flags) const
{
    if (!(flags & BxDF_SamplingFlags::Reflection) || !SameHemisphere(wo, wi))
    {
        return 0;
    }

    return CosineSampleHemispherePDF(AbsCosTheta(wi));
}

bool LambertianBxDF::Sample_f(BSDFSample* sample, Vec3 wo, Float u0, Point2 u12, BxDF_SamplingFlags flags) const
{
    if (!(flags & BxDF_SamplingFlags::Reflection))
    {
        return false;
    }

    Vec3 wi = CosineSampleHemisphere(u12);
    Float pdf = CosineSampleHemispherePDF(CosTheta(wi));
    if (wo.z < 0)
    {
        wi.z = -wi.z;
    }

    *sample = BSDFSample(r * inv_pi, wi, pdf, BxDF_Flags::DiffuseReflection);
    return true;
}

} // namespace bulbit
