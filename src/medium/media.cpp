#include "bulbit/media.h"
#include "bulbit/sampling.h"

namespace bulbit
{

Float HenyeyGreensteinPhaseFunction::p(Vec3 wo, Vec3 wi) const
{
    return HenyeyGreenstein(Dot(wo, wi), g);
}

Float HenyeyGreensteinPhaseFunction::PDF(Vec3 wo, Vec3 wi) const
{
    return HenyeyGreenstein(Dot(wo, wi), g);
}

bool HenyeyGreensteinPhaseFunction::Sample_p(PhaseFunctionSample* sample, Vec3 wo, Point2 u) const
{
    Float pdf;
    Vec3 wi = SampleHenyeyGreenstein(wo, g, u, &pdf);
    *sample = PhaseFunctionSample(pdf, wi, pdf);
    return true;
}

} // namespace bulbit
