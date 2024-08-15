#pragma once

#include "common.h"
#include "spectrum.h"

#include <complex>

namespace bulbit
{

inline bool Refract(Vec3* wt, Vec3 wi, Vec3 n, Float eta, Float* eta_p)
{
    Float cos_theta_i = Dot(n, wi);

    // Potentially flip interface orientation
    if (cos_theta_i < 0)
    {
        eta = 1 / eta;
        cos_theta_i = -cos_theta_i;
        n = -n;
    }

    Float sin2_theta_i = std::max<Float>(0, 1 - Sqr(cos_theta_i));
    Float sin2_theta_t = sin2_theta_i / Sqr(eta);

    // Case of total internal reflection
    if (sin2_theta_t >= 1)
    {
        return false;
    }

    Float cos_theta_t = std::sqrt(1 - sin2_theta_t);

    Float inv_eta = 1 / eta;
    *wt = -wi * inv_eta + (cos_theta_i * inv_eta - cos_theta_t) * n;

    if (eta_p)
    {
        *eta_p = eta;
    }

    return true;
}

inline Float SchlickR0(Float eta)
{
    return Sqr(eta - 1) / Sqr(eta + 1);
}

inline Spectrum FresnelSchlick(const Spectrum& r0, Float cos_theta)
{
    return r0 + (Spectrum(1) - r0) * std::pow<Float>(1 - cos_theta, 5);
}

inline Float FresnelDielectric(Float cos_theta_i, Float eta)
{
    cos_theta_i = Clamp(cos_theta_i, -1, 1);

    // Potentially flip interface orientation for Fresnel equations
    if (cos_theta_i < 0)
    {
        eta = 1 / eta;
        cos_theta_i = -cos_theta_i;
    }

    // Compute cos_theta_t for Fresnel equations using Snell's law
    Float sin2_theta_i = 1 - Sqr(cos_theta_i);
    Float sin2_theta_t = sin2_theta_i / Sqr(eta);
    if (sin2_theta_t >= 1)
    {
        // Total internal reflection
        return 1;
    }

    Float cos_theta_t = std::sqrt(std::max<Float>(0, 1 - sin2_theta_t));

    Float r_parl = (eta * cos_theta_i - cos_theta_t) / (eta * cos_theta_i + cos_theta_t);
    Float r_perp = (cos_theta_i - eta * cos_theta_t) / (cos_theta_i + eta * cos_theta_t);

    return (Sqr(r_parl) + Sqr(r_perp)) / 2;
}

template <typename T>
inline std::complex<T> SqrtComplex(const std::complex<T>& z)
{
    T n = std::abs(z);
    T t1 = std::sqrt(T(0.5) * (n + std::abs(z.real())));
    T t2 = T(0.5) * z.imag() / t1;

    if (n == 0)
    {
        return 0;
    }

    if (z.real() >= 0)
    {
        return { t1, t2 };
    }
    else
    {
        return { std::abs(t2), std::copysign(t1, z.imag()) };
    }
}

inline Float FresnelComplex(Float cos_theta_i, const std::complex<Float>& eta /* == eta_t / eta_i */)
{
    using Complex = std::complex<Float>;

    // It's essentially the same calculation as the dielectric case,
    // but only the absorption coefficient k is added.

    cos_theta_i = Clamp(cos_theta_i, 0, 1);

    // Compute complex cos_theta_t for Fresnel equations using Snell's law
    Float sin2_theta_i = 1 - Sqr(cos_theta_i);
    Complex sin2_theta_t = sin2_theta_i / Sqr(eta);
    Complex cos_theta_t = SqrtComplex(Complex(1) - sin2_theta_t);

    Complex r_parl = (eta * cos_theta_i - cos_theta_t) / (eta * cos_theta_i + cos_theta_t);
    Complex r_perp = (cos_theta_i - eta * cos_theta_t) / (cos_theta_i + eta * cos_theta_t);

    return (std::norm(r_parl) + std::norm(r_perp)) / 2;
}

inline Spectrum FresnelComplex(Float cos_theta_i, const Spectrum& eta, const Spectrum& k)
{
    Spectrum result;
    for (int32 i = 0; i < Spectrum::num_spectral_samples; ++i)
    {
        result[i] = FresnelComplex(cos_theta_i, std::complex<Float>(eta[i], k[i]));
    }

    return result;
}

// https://www.pbr-book.org/3ed-2018/Volume_Scattering/The_BSSRDF#FresnelMoment1
// Precomputed value of
// F(\eta) = \int_0^{\frac{\pi}{2}} F_r(cos\theta, \eta) sin\theta cos\theta d\theta
inline Float FresnelMoment1(Float eta)
{
    Float eta2 = eta * eta;
    Float eta3 = eta2 * eta;
    Float eta4 = eta3 * eta;
    Float eta5 = eta4 * eta;

    if (eta < 1)
    {
        return 0.45966f - 1.73965f * eta + 3.37668f * eta2 - 3.904945f * eta3 + 2.49277f * eta4 - 0.68441f * eta5;
    }
    else
    {
        return -4.61686f + 11.1136f * eta - 10.4646f * eta2 + 5.11455f * eta3 - 1.27198f * eta4 + 0.12746f * eta5;
    }
}

} // namespace bulbit
