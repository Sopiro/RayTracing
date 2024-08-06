#include "bulbit/bssrdfs.h"

namespace bulbit
{

Float GaussianBSSRDF::MaxSr(int32 wavelength) const
{
    return Sample_Sr(wavelength, 1 - 0.999f);
}

Spectrum GaussianBSSRDF::Sr(Float r) const
{
    return R * PDF_Sr(r);
}

Float GaussianBSSRDF::Sample_Sr(int32 wavelength, Float u) const
{
    return sigma[wavelength] * std::sqrt(-2 * std::log(std::max<Float>(u, 1e-8)));
}

Spectrum GaussianBSSRDF::PDF_Sr(Float r) const
{
    Spectrum sigma2 = Sqr(sigma);
    return Exp(-Sqr(r) / (2 * sigma2)) / (two_pi * sigma2);
}

} // namespace bulbit
