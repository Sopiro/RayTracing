#include "bulbit/bssrdfs.h"

namespace bulbit
{

Float DisneyBSSRDF::MaxSr(int32 wavelength) const
{
    return 0.5 * Sample_Sr(wavelength, 0.999f) + 0.5 * Sample_Sr(wavelength, 0.249f);
}

Spectrum DisneyBSSRDF::Sr(Float r) const
{
    if (r < 1e-6f)
    {
        r = 1e-6f;
    }

    return R * (Exp(-Spectrum(r) / d) + Exp(-Spectrum(r) / (3 * d))) / (8 * pi * d * r);
}

Float DisneyBSSRDF::Sample_Sr(int32 wavelength, Float u) const
{
    if (u < 0.25f)
    {
        // Sample the first exponential
        u = std::min<Float>(u * 4, 1 - epsilon); // renormalize to [0,1)
        return d[wavelength] * std::log(1 / (1 - u));
    }
    else
    {
        // Second exponenital
        u = std::min<Float>((u - .25f) / .75f, 1 - epsilon); // normalize to [0,1)
        return 3 * d[wavelength] * std::log(1 / (1 - u));
    }
}

Spectrum DisneyBSSRDF::PDF_Sr(Float r) const
{
    if (r < 1e-6f)
    {
        r = 1e-6f;
    }

    // Weight the two individual PDFs
    return (0.25f * Exp(-r / d) / (2 * pi * d * r) + 0.75f * Exp(-r / (3 * d)) / (6 * pi * d * r));
}

} // namespace bulbit
