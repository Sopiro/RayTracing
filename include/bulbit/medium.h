#pragma once

#include "math.h"

namespace bulbit
{

struct PhaseFunctionSample
{
    Float p;
    Vec3 wi;
    Float pdf;
};

class PhaseFunction
{
public:
    virtual ~PhaseFunction() = default;

    virtual Float p(Vec3 wo, Vec3 wi) const = 0;
    virtual Float PDF(Vec3 wo, Vec3 wi) const = 0;

    virtual bool Sample_p(PhaseFunctionSample* sample, Vec3 wo, Point2 u) const = 0;
};

} // namespace bulbit
