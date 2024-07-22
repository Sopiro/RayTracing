#pragma once

#include "medium.h"
#include "random.h"
#include "sampling.h"

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

class HomogeneousMajorantIterator : public RayMajorantIterator
{
public:
    HomogeneousMajorantIterator()
        : called{ true }
    {
    }

    HomogeneousMajorantIterator(Float t_min, Float t_max, Spectrum sigma_maj)
        : segment{ t_min, t_max, sigma_maj }
        , called{ false }
    {
    }

    virtual bool Next(RayMajorantSegment* next_segment) override
    {
        if (called)
        {
            return false;
        }

        *next_segment = segment;

        called = true;
        return true;
    }

private:
    RayMajorantSegment segment;
    bool called;
};

class HomogeneousMedium : public Medium
{
public:
    using MajorantIterator = HomogeneousMajorantIterator;

    HomogeneousMedium(Spectrum sigma_a, Spectrum sigma_s, Spectrum Le, Float g)
        : Medium(0)
        , sigma_a{ sigma_a }
        , sigma_s{ sigma_s }
        , Le{ Le }
        , phase{ g }
    {
    }

    bool IsEmissive() const
    {
        return !Le.IsBlack();
    }

    MediumSample SamplePoint(Point3 p) const
    {
        return MediumSample{ sigma_a, sigma_s, Le, &phase };
    }

    HomogeneousMajorantIterator SampleRay(Ray ray, Float t_max) const
    {
        return HomogeneousMajorantIterator(Float(0), t_max, sigma_a + sigma_s);
    }

    RayMajorantIterator* SampleRay(Ray ray, Float t_max, Allocator& alloc) const
    {
        return alloc.new_object<HomogeneousMajorantIterator>(Float(0), t_max, sigma_a + sigma_s);
    }

private:
    Spectrum sigma_a, sigma_s, Le;
    HenyeyGreensteinPhaseFunction phase;
};

} // namespace bulbit
