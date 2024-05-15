#include "bulbit/bxdfs.h"
#include "bulbit/microfacet.h"

namespace bulbit
{

Spectrum UnrealBxDF::f(const Vec3& wo, const Vec3& wi) const
{
    if (!SameHemisphere(wo, wi))
    {
        return Spectrum::black;
    }

    Vec3 v = wi;    // incident
    Vec3 l = wo;    // outgoing
    Vec3 h = v + l; // half

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
    // Spectrum f_s = F * (D * G) / (Float(4.0) * NoV * NoL);
    Spectrum f_d = (Spectrum(1) - F) * (1 - metallic) * (basecolor * inv_pi);

    return f_d + f_s;
}

bool UnrealBxDF::Sample_f(BSDFSample* sample, Vec3 wo, Float u0, Point2 u12, BxDF_SamplingFlags sample_flags) const
{
    if (!(sample_flags & BxDF_SamplingFlags::Reflection))
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

    *sample = BSDFSample(f(wo, wi), wi, PDF(wo, wi, sample_flags), flag);
    return true;
}

Float UnrealBxDF::PDF(Vec3 wo, Vec3 wi, BxDF_SamplingFlags sample_flags) const
{
    if (!(sample_flags & BxDF_SamplingFlags::Reflection) || !SameHemisphere(wo, wi))
    {
        return 0;
    }

    Float alpha2 = alpha * alpha;

    Vec3 h = Normalize(wo + wi);
    Float NoH = CosTheta(h);
    Float LoH = CosTheta(wi);
    Float spec_w = D_GGX(NoH, alpha2) * G1_Smith(LoH, alpha2) / std::fmax(4 * LoH, Float(0.0));

    Float diff_w = LoH * inv_pi;

    return (1 - t) * diff_w + t * spec_w;
}

void UnrealBxDF::Regularize()
{
    if (alpha < Float(0.3)) alpha = Clamp(2 * alpha, Float(0.1), Float(0.3));
}

} // namespace bulbit
