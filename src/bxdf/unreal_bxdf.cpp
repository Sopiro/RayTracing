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

    Vec3 v = wi;
    Vec3 l = wo;
    Vec3 h = v + l;

    Float NoV = CosTheta(v);
    Float NoL = CosTheta(l);

    if (NoV <= 0 || NoL <= 0 || h == Vec3::zero)
    {
        return RGBSpectrum::black;
    }

    h.Normalize();

    Float NoH = CosTheta(h);
    Float VoH = Dot(v, h);

    Float alpha2 = alpha * alpha;

    Spectrum f0 = F0(basecolor, metallic);
    Spectrum F = F_Schlick(f0, VoH);
    Float D = D_GGX(NoH, alpha2);
    // Float G = G2_Smith_Correlated(NoV, NoL, alpha2);
    Float V = V_Smith_Correlated(NoV, NoL, alpha2);

    Spectrum f_s = F * (D * V);
    // Spectrum f_s = F * (D * G) / (4.0f * NoV * NoL);
    Spectrum f_d = (Spectrum(1) - F) * (1 - metallic) * (basecolor * inv_pi);

    return f_d + f_s;
}

Float UnrealBxDF::PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags flags) const
{
    if (!(flags & BxDF_SamplingFlags::Reflection) || !SameHemisphere(wo, wi))
    {
        return 0;
    }

    Float alpha2 = alpha * alpha;

    Vec3 h = Normalize(wo + wi);
    Float NoH = CosTheta(h);
    Float LoH = CosTheta(wi);
    Float spec_w = D_GGX(NoH, alpha2) * G1_Smith(LoH, alpha2) / std::fmax(4 * LoH, 0.0f);

    Float diff_w = LoH * inv_pi;

    return (1 - t) * diff_w + t * spec_w;
}

bool UnrealBxDF::Sample_f(BSDFSample* sample, Vec3 wo, Float u0, Point2 u12, BxDF_SamplingFlags flags) const
{
    if (!(flags & BxDF_SamplingFlags::Reflection))
    {
        return false;
    }

    Vec3 wi;
    BxDF_Flags flag;

    if (u0 < t)
    {
#if 1
        Vec3 h = Sample_GGX_VNDF_Dupuy_Benyoub(wo, alpha, alpha, u12);
#else
        Vec3 h = Sample_GGX_VNDF_Heitz(wo, alpha, alpha, u12);
#endif
        wi = Reflect(wo, h);
        flag = BxDF_Flags::GlossyReflection;
    }
    else
    {
        wi = CosineSampleHemisphere(u12);
        flag = BxDF_Flags::DiffuseReflection;
    }

    if (wi.z < 0)
    {
        return false;
    }

    *sample = BSDFSample(f(wo, wi), wi, PDF(wo, wi, flags), flag);
    return true;
}

void UnrealBxDF::Regularize()
{
    if (alpha < 0.3f) alpha = Clamp(2 * alpha, 0.1f, 0.3f);
}

} // namespace bulbit
