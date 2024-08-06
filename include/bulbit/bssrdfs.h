#pragma once

#include "bssrdf.h"

namespace bulbit
{

class DisneyBSSRDF : public SeparableBSSRDF
{
public:
    DisneyBSSRDF(const Spectrum& R, const Spectrum& d, const Intersection& po, const Vec3& wo, Float eta)
        : SeparableBSSRDF(po, wo, eta)
        , R{ R }
        , d{ d }
    {
    }

    virtual Float MaxSr(int32 wavelength) const override;
    virtual Spectrum Sr(Float d) const override;
    virtual Float Sample_Sr(int32 wavelength, Float u) const override;
    virtual Spectrum PDF_Sr(Float r) const override;

private:
    Spectrum R, d;
};

constexpr size_t max_bssrdf_size = std::max({ sizeof(DisneyBSSRDF) });

} // namespace bulbit
