#include "bulbit/bxdfs.h"
#include "bulbit/frame.h"
#include "bulbit/sampling.h"

namespace bulbit
{

Spectrum UnrealBxDF::f(const Vec3& wo, const Vec3& wi) const
{
    if (!SameHemisphere(wo, wi))
    {
        return Spectrum::black;
    }

    Float cos_theta_o = AbsCosTheta(wo);
    Float cos_theta_i = AbsCosTheta(wi);
    if (cos_theta_i == 0 || cos_theta_o == 0)
    {
        RGBSpectrum::black;
    }

    Vec3 wm = wo + wi;
    if (Length2(wm) == 0)
    {
        return Spectrum::black;
    }
    wm.Normalize();

    Spectrum f0 = F0(basecolor, metallic);
    Spectrum F = F_Schlick(f0, Dot(wi, wm));

    Spectrum f_s = F * mf.D(wm) * mf.G(wo, wi) / (4 * cos_theta_i * cos_theta_o);
    Spectrum f_d = (Spectrum(1) - F) * (1 - metallic) * (basecolor * inv_pi);

    return f_d + f_s;
}

Float UnrealBxDF::PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags flags) const
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

    Float p_s = mf.PDF(wo, wm) / (4 * AbsDot(wo, wm));
    Float p_d = AbsCosTheta(wi) * inv_pi;

    return t * p_s + (1 - t) * p_d;
}

bool UnrealBxDF::Sample_f(BSDFSample* sample, Vec3 wo, Float u0, Point2 u12, BxDF_SamplingFlags flags) const
{
    if (!(flags & BxDF_SamplingFlags::Reflection))
    {
        return false;
    }

    if (mf.EffectivelySmooth())
    {
        // Sample perfect specular conductor BRDF
        Vec3 wi(-wo.x, -wo.y, wo.z);

        Spectrum f0 = F0(basecolor, metallic);
        Spectrum F = F_Schlick(f0, AbsCosTheta(wi));

        Spectrum f_s = F / AbsCosTheta(wi);
        Spectrum f_d = (Spectrum(1) - F) * (1 - metallic) * (basecolor * inv_pi);

        *sample = BSDFSample(f_s + f_d, wi, 1, BxDF_Flags::SpecularReflection);
        return true;
    }

    if (wo.z == 0)
    {
        return false;
    }

    BxDF_Flags flag;
    Vec3 wm, wi;
    if (u0 < t)
    {
        // Sample glossy
        wm = mf.Sample_Wm(wo, u12);
        wi = Reflect(wo, wm);

        if (!SameHemisphere(wo, wi))
        {
            return false;
        }

        flag = BxDF_Flags::GlossyReflection;
    }
    else
    {
        // Sample diffuse
        wi = CosineSampleHemisphere(u12);
        wm = Normalize(wi + wo);

        flag = BxDF_Flags::DiffuseReflection;
    }

    Float cos_theta_o = AbsCosTheta(wo);
    Float cos_theta_i = AbsCosTheta(wi);
    if (cos_theta_i == 0 || cos_theta_o == 0)
    {
        false;
    }

    Spectrum f0 = F0(basecolor, metallic);
    Spectrum F = F_Schlick(f0, Dot(wi, wm));

    Spectrum f_s = F * mf.D(wm) * mf.G(wo, wi) / (4 * cos_theta_i * cos_theta_o);
    Spectrum f_d = (Spectrum(1) - F) * (1 - metallic) * (basecolor * inv_pi);

    Float p_s = mf.PDF(wo, wm) / (4 * AbsDot(wo, wm));
    Float p_d = cos_theta_i * inv_pi;

    *sample = BSDFSample(f_s + f_d, wi, t * p_s + (1 - t) * p_d, flag);

    return true;
}

void UnrealBxDF::Regularize()
{
    mf.Regularize();
}

} // namespace bulbit
