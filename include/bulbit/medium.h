#pragma once

#include "math.h"
#include "ray.h"
#include "spectrum.h"

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

struct MediumSample
{
    Spectrum sigma_a, sigma_s;
    Spectrum Le;
    const PhaseFunction* phase;
};

struct RayMajorantSegment
{
    Float t_min, t_max;
    Spectrum sigma_maj;
};

class RayMajorantIterator
{
public:
    virtual bool Next(RayMajorantSegment* next_segment) = 0;
};

class Medium
{
public:
    virtual bool IsEmissive() const = 0;
    virtual MediumSample SamplePoint(Point3 p) const = 0;
    virtual RayMajorantIterator* SampleRay(Ray ray, Float t_max, Allocator& alloc) const = 0;
};

struct MediumInterface
{
    MediumInterface()
        : inside{ nullptr }
        , outside{ nullptr }
    {
    }

    MediumInterface(Medium* medium)
        : inside{ medium }
        , outside{ medium }
    {
    }

    MediumInterface(Medium* inside, Medium* outside)
        : inside{ inside }
        , outside{ outside }
    {
    }

    bool IsMediumTransition() const
    {
        return inside != outside;
    }

    Medium* inside;
    Medium* outside;
};

} // namespace bulbit
