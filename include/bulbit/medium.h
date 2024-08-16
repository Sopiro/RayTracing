#pragma once

#include "dynamic_dispatcher.h"
#include "random.h"
#include "ray.h"
#include "sampling.h"
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

using Media = TypePack<class HomogeneousMedium>;

class Medium : public DynamicDispatcher<Media>
{
public:
    Medium(int8 index)
        : DynamicDispatcher(index)
    {
    }

    bool IsEmissive() const;
    MediumSample SamplePoint(Point3 p) const;
    RayMajorantIterator* SampleRay(Ray ray, Float t_max, Allocator& alloc) const;
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

template <typename F>
Spectrum Sample_MajorantTransmittance(const Medium* medium, int32 wavelength, Ray ray, Float t_max, Float u, RNG& rng, F callback)
{
    return medium->Dispatch([&](auto m) -> Spectrum {
        return Sample_MajorantTransmittance(m, wavelength, ray, t_max, u, rng, callback);
    });
}

// bool callback(Point3 p, MediumSample ms, Spectrum sigma_maj, Spectrum T_maj);
template <typename MediumType, typename F>
Spectrum Sample_MajorantTransmittance(
    const MediumType* medium, int32 wavelength, Ray ray, Float t_max, Float u, RNG& rng, F callback
)
{
    t_max *= ray.d.Normalize();

    typename MediumType::MajorantIterator iter = medium->SampleRay(ray, t_max);

    Spectrum T_maj(1);
    bool done = false;
    while (!done)
    {
        RayMajorantSegment segment;
        if (!iter.Next(&segment))
        {
            // No medium segment left, return majorant transmittance
            return T_maj;
        }

        if (segment.sigma_maj[wavelength] == 0)
        {
            Float dt = segment.t_max - segment.t_min;
            if (dt == infinity)
            {
                dt = max_float;
            }

            T_maj *= Exp(-dt * segment.sigma_maj);
            continue;
        }

        Float t_min = segment.t_min;

        while (true)
        {
            Float t = t_min + SampleExponential(u, segment.sigma_maj[wavelength]);
            u = rng.NextFloat();

            if (t < segment.t_max)
            {
                // Sampled distance is inside the segment's extent
                // Return medium sample to the callback
                T_maj *= Exp(-(t - t_min) * segment.sigma_maj);
                Point3 p = ray.At(t);
                MediumSample ms = medium->SamplePoint(p);
                if (!callback(p, ms, segment.sigma_maj, T_maj))
                {
                    done = true;
                    break;
                }

                // Restart sampling at the sampled point
                T_maj = Spectrum(1);
                t_min = t;
            }
            else
            {
                // Sampled distance is past the end
                // Accumulate majorant transmittance of current segment and continue to the next segment
                Float dt = segment.t_max - t_min;
                if (dt == infinity)
                {
                    dt = max_float;
                }

                T_maj *= Exp(-dt * segment.sigma_maj);
                break;
            }
        }
    }

    return Spectrum(1);
}

} // namespace bulbit
