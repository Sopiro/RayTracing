#include "bulbit/bxdfs.h"
#include "bulbit/scattering.h"

namespace bulbit
{

Spectrum ThinDielectricBxDF::f(const Vec3& wo, const Vec3& wi) const
{
    return Spectrum::black;
}

Float ThinDielectricBxDF::PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags flags) const
{
    return 0;
}

bool ThinDielectricBxDF::Sample_f(BSDFSample* sample, Vec3 wo, Float u0, Point2 u12, BxDF_SamplingFlags flags) const
{
    Float R = FresnelDielectric(AbsCosTheta(wo), eta);
    Float T = 1 - R;

    // Update R and T accounting for scattering at thin dielectric interfaces
    if (R < 1)
    {
        R += Sqr(T) * R / (1 - Sqr(R));
        T = 1 - R;
    }

    Float pr = R;
    Float pt = T;
    if (!(flags & BxDF_SamplingFlags::Reflection)) pr = 0;
    if (!(flags & BxDF_SamplingFlags::Transmission)) pt = 0;
    if (pr == 0 && pt == 0)
    {
        return false;
    }

    if (u0 < pr / (pr + pt))
    {
        // Sample perfect specular dielectric BRDF
        Vec3 wi(-wo.x, -wo.y, wo.z);
        Spectrum fr(R / AbsCosTheta(wi));
        *sample = BSDFSample(fr, wi, pr / (pr + pt), BxDF_Flags::SpecularReflection);
    }
    else
    {
        // Sample perfect specular transmission at thin dielectric interface
        Vec3 wi = -wo;
        Spectrum ft(T / AbsCosTheta(wi));
        *sample = BSDFSample(ft, wi, pt / (pr + pt), BxDF_Flags::SpecularTransmission);
    }

    return true;
}

} // namespace bulbit
