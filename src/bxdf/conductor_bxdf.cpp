#include "bulbit/bxdfs.h"
#include "bulbit/scattering.h"

#define SIMPLE_FRESNEL 1

namespace bulbit
{

Spectrum ConductorBxDF::f(const Vec3& wo, const Vec3& wi) const
{
    if (!SameHemisphere(wo, wi) || mf.EffectivelySmooth())
    {
        return Spectrum::black;
    }

    Float cos_theta_o = AbsCosTheta(wo);
    Float cos_theta_i = AbsCosTheta(wi);
    if (cos_theta_i == 0 || cos_theta_o == 0)
    {
        return Spectrum::black;
    }

    Vec3 wm = wo + wi;
    if (Length2(wm) == 0)
    {
        return Spectrum::black;
    }
    wm.Normalize();

    // Evaluate Fresnel term for conductor
    Spectrum F = FresnelComplex(AbsDot(wo, wm), eta, k);
    return mf.D(wm) * F * mf.G(wo, wi) / (4 * cos_theta_i * cos_theta_o);
}

Float ConductorBxDF::PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags flags) const
{
    if (!(flags & BxDF_SamplingFlags::Reflection))
    {
        return 0;
    }

    if (!SameHemisphere(wo, wi) || mf.EffectivelySmooth())
    {
        return 0;
    }

    Vec3 wm = wo + wi;
    if (Length2(wm) == 0)
    {
        return 0;
    }
    wm.Normalize();

    if (Dot(wm, z_axis) < 0)
    {
        wm.Negate();
    }

    return mf.PDF(wo, wm) / (4 * AbsDot(wo, wm));
}

bool ConductorBxDF::Sample_f(BSDFSample* sample, Vec3 wo, Float u0, Point2 u12, BxDF_SamplingFlags flags) const
{
    BulbitNotUsed(u0);

    if (!(flags & BxDF_SamplingFlags::Reflection))
    {
        return false;
    }

    if (mf.EffectivelySmooth())
    {
        // Sample perfect specular conductor BRDF
        Vec3 wi(-wo.x, -wo.y, wo.z);
        Spectrum f = FresnelComplex(AbsCosTheta(wi), eta, k) / AbsCosTheta(wi);
        *sample = BSDFSample(f, wi, 1, BxDF_Flags::SpecularReflection);
        return true;
    }

    // Sample rough conductor BRDF
    if (wo.z == 0)
    {
        return false;
    }

    Vec3 wm = mf.Sample_Wm(wo, u12);
    Vec3 wi = Reflect(wo, wm);

    if (!SameHemisphere(wo, wi))
    {
        return false;
    }

    Float cos_theta_o = AbsCosTheta(wo);
    Float cos_theta_i = AbsCosTheta(wi);
    if (cos_theta_i == 0 || cos_theta_o == 0)
    {
        return false;
    }

    Float pdf = mf.PDF(wo, wm) / (4 * AbsDot(wo, wm));

    // Evaluate Fresnel term for conductor
    Spectrum F = FresnelComplex(AbsDot(wo, wm), eta, k);
    Spectrum f = mf.D(wm) * F * mf.G(wo, wi) / (4 * cos_theta_i * cos_theta_o);
    *sample = BSDFSample(f, wi, pdf, BxDF_Flags::GlossyReflection);
    return true;
}

void ConductorBxDF::Regularize()
{
    mf.Regularize();
}

} // namespace bulbit
