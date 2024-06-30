#pragma once

#include "medium.h"

namespace bulbit
{

class HenyeyGreensteinPhaseFunction : public PhaseFunction
{
public:
    HenyeyGreensteinPhaseFunction(Float g)
        : g(g)
    {
    }

    virtual Float p(Vec3 wo, Vec3 wi) const override;
    virtual Float PDF(Vec3 wo, Vec3 wi) const override;

    virtual bool Sample_p(PhaseFunctionSample* sample, Vec3 wo, Point2 u) const override;

private:
    Float g;
};

} // namespace bulbit
