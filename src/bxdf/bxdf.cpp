#include "bulbit/bxdf.h"
#include "bulbit/frame.h"
#include "bulbit/sampling.h"

namespace bulbit
{

Spectrum BxDF::rho(Vec3 wo, std::span<const Float> uc, std::span<const Point2> u) const
{
    Spectrum r(0);
    for (size_t i = 0; i < uc.size(); ++i)
    {
        BSDFSample sample;
        if (Sample_f(&sample, wo, uc[i], u[i]))
        {
            r += sample.f * AbsCosTheta(sample.wi) / sample.pdf;
        }
    }
    return r / uc.size();
}

Spectrum BxDF::rho(std::span<const Point2> u1, std::span<const Float> uc, std::span<const Point2> u2) const
{
    Spectrum r(0);
    for (size_t i = 0; i < uc.size(); ++i)
    {
        Vec3 wo = SampleUniformHemisphere(u1[i]);
        if (wo.z == 0)
        {
            continue;
        }

        Float pdf_o = UniformHemispherePDF();
        BSDFSample sample;
        if (Sample_f(&sample, wo, uc[i], u2[i]))
        {
            r += sample.f * AbsCosTheta(sample.wi) * AbsCosTheta(wo) / (pdf_o * sample.pdf);
        }
    }
    return r / (pi * uc.size());
}

} // namespace bulbit
