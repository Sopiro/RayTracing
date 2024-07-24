#pragma once

#include "common.h"

namespace bulbit
{

class Sampler
{
public:
    Sampler(int32 samples_per_pixel);
    virtual ~Sampler() = default;

    virtual void StartPixelSample();
    virtual bool StartNextPixelSample();

    virtual Float Next1D() = 0;
    virtual Point2 Next2D() = 0;

    virtual std::unique_ptr<Sampler> Clone(int32 seed) const = 0;

    const int32 samples_per_pixel;

protected:
    int32 current_pixel_sample;
};

inline Sampler::Sampler(int32 spp)
    : samples_per_pixel{ spp }
{
}

inline void Sampler::StartPixelSample()
{
    current_pixel_sample = 0;
}

inline bool Sampler::StartNextPixelSample()
{
    return ++current_pixel_sample != samples_per_pixel;
}

} // namespace bulbit
