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
    HomogeneousMedium(Spectrum sigma_a, Spectrum sigma_s, Spectrum Le, Float g);

    virtual bool IsEmissive() const override;
    virtual MediumSample SamplePoint(Point3 p) const override;
    virtual RayMajorantIterator* SampleRay(Ray ray, Float t_max, Allocator& alloc) const override;

private:
    Spectrum sigma_a, sigma_s, Le;
    HenyeyGreensteinPhaseFunction phase;
};

constexpr inline size_t max_majorant_iterator_size = std::max({ sizeof(HomogeneousMajorantIterator) });
// bool callback(Point3 p, MediumSample ms, Spectrum sigma_maj, Spectrum T_maj);

template <typename F>
Spectrum Sample_MajorantTransmittance(const Medium* medium, int32 wavelength, Ray ray, Float t_max, Float u, RNG& rng, F callback)
{
    t_max *= ray.d.Normalize();

    int8 mem[max_majorant_iterator_size];
    Resource res(mem, sizeof(mem));
    Allocator alloc(&res);
    RayMajorantIterator* iter = medium->SampleRay(ray, t_max, alloc);

    Spectrum T_maj(1);
    bool done = false;
    while (!done)
    {
        RayMajorantSegment segment;
        if (!iter->Next(&segment))
        {
            // No medium segment left, return majorant transmittance
            return T_maj;
        }

        if (segment.sigma_maj[wavelength] == 0)
        {
            Float dt = segment.t_max - segment.t_min;
            if (dt == infinity)
            {
                dt = max_value;
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
                    dt = max_value;
                }

                T_maj *= Exp(-dt * segment.sigma_maj);
                break;
            }
        }
    }

    return Spectrum(1);
}

} // namespace bulbit
