#include "bulbit/bxdfs.h"
#include "bulbit/frame.h"
#include "bulbit/sampling.h"

namespace bulbit
{

Spectrum SpecularReflectionBxDF::f(const Vec3& wo, const Vec3& wi) const
{
    return Spectrum::black;
}

Float SpecularReflectionBxDF::PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags flags) const
{
    return 0;
}

bool SpecularReflectionBxDF::Sample_f(BSDFSample* sample, Vec3 wo, Float u0, Point2 u12, BxDF_SamplingFlags flags) const
{
    if (!(flags & BxDF_SamplingFlags::Reflection))
    {
        return false;
    }

    Vec3 wi(-wo.x, -wo.y, wo.z);
    *sample = BSDFSample(r / AbsCosTheta(wi), wi, 1, BxDF_Flags::SpecularReflection);
    return true;
}

} // namespace bulbit
