#pragma once

#include "common.h"

namespace spt
{

struct Distribution1D
{
    Distribution1D(const Float* f, int32 n)
        : func(f, f + n)
        , cdf(n + 1)
    {
        cdf[0] = 0;
        for (int32 i = 1; i < n + 1; ++i)
        {
            cdf[i] = cdf[i - 1] + func[i - 1] / n;
        }

        funcIntegral = cdf[n];
        if (funcIntegral == 0)
        {
            for (int32 i = 1; i < n + 1; ++i)
            {
                cdf[i] = Float(i) / Float(n);
            }
        }
        else
        {
            // Normalization
            for (int32 i = 1; i < n + 1; ++i)
            {
                cdf[i] /= funcIntegral;
            }
        }
    }

    Float SampleContinuous(Float u, Float* pdf, int32* off = nullptr) const
    {
        // Find the starting offset of cdf
        int32 offset = FindInterval(cdf.size(), [&](int index) { return cdf[index] <= u; });

        if (off)
        {
            *off = offset;
        }

        if (pdf)
        {
            *pdf = func[offset] / funcIntegral;
        }

        Float du = (u - cdf[offset]) / (cdf[offset + 1] - cdf[offset]);

        return (offset + du) / Count();
    }

    int32 SampleDiscrete(Float u, Float* pdf = nullptr, Float* u_remapped = nullptr) const
    {
        int32 offset = FindInterval(cdf.size(), [&](int32 index) { return cdf[index] <= u; });

        if (pdf)
        {
            *pdf = func[offset] / (funcIntegral * Count());
        }

        if (u_remapped)
        {
            *u_remapped = (u - cdf[offset]) / (cdf[offset + 1] - cdf[offset]);
        }

        return offset;
    }

    int32 Count() const
    {
        return func.size();
    }

    Float DiscretePDF(int32 index) const
    {
        return func[index] / (funcIntegral * Count());
    }

    std::vector<Float> func, cdf;
    Float funcIntegral;
};

} // namespace spt
