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

        func_integral = cdf[n];
        if (func_integral == 0)
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
                cdf[i] /= func_integral;
            }
        }
    }

    Float SampleContinuous(Float* pdf, Float u, int32* off = nullptr) const
    {
        // Find the starting offset of cdf
        int32 offset = FindInterval((int32)cdf.size(), [&](int32 index) { return cdf[index] <= u; });

        if (off)
        {
            *off = offset;
        }

        if (pdf)
        {
            *pdf = func[offset] / func_integral;
        }

        Float du = (u - cdf[offset]) / (cdf[offset + 1] - cdf[offset]);

        return (offset + du) / Count();
    }

    int32 SampleDiscrete(Float u, Float* pdf = nullptr, Float* u_remapped = nullptr) const
    {
        int32 offset = FindInterval((int32)cdf.size(), [&](int32 index) { return cdf[index] <= u; });

        if (pdf)
        {
            *pdf = func[offset] / (func_integral * Count());
        }

        if (u_remapped)
        {
            *u_remapped = (u - cdf[offset]) / (cdf[offset + 1] - cdf[offset]);
        }

        return offset;
    }

    int32 Count() const
    {
        return (int32)func.size();
    }

    Float DiscretePDF(int32 index) const
    {
        return func[index] / (func_integral * Count());
    }

    std::vector<Float> func, cdf;
    Float func_integral;
};

class Distribution2D
{
public:
    Distribution2D(const Float* func, int32 nu, int32 nv)
    {
        conditional_v.reserve(nv);
        for (int32 v = 0; v < nv; ++v)
        {
            conditional_v.emplace_back(new Distribution1D(&func[v * nu], nu));
        }

        std::vector<Float> marginal_func(nv);
        for (int32 v = 0; v < nv; ++v)
        {
            marginal_func[v] = conditional_v[v]->func_integral;
        }

        marginal.reset(new Distribution1D(&marginal_func[0], nv));
    }

    Point2 SampleContinuous(Float* pdf, const Point2& u) const
    {
        Float pdfs[2];
        int32 v;

        Float d1 = marginal->SampleContinuous(&pdfs[1], u[1], &v);
        Float d0 = conditional_v[v]->SampleContinuous(&pdfs[0], u[0]);

        *pdf = pdfs[0] * pdfs[1];
        return Point2(d0, d1);
    }

    Float Pdf(const Point2& p) const
    {
        int32 w = conditional_v[0]->Count();
        int32 h = marginal->Count();

        int32 iu = Clamp(int32(p[0] * w), 0, w - 1);
        int32 iv = Clamp(int32(p[1] * h), 0, h - 1);

        return conditional_v[iv]->func[iu] / marginal->func_integral;
    }

private:
    std::vector<std::unique_ptr<Distribution1D>> conditional_v; // p(u|v)
    std::unique_ptr<Distribution1D> marginal;
};

} // namespace spt
