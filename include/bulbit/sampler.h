#pragma once

#include "common.h"

namespace bulbit
{

class Sampler
{
public:
    Sampler(int32 samples_per_pixel);
    virtual ~Sampler() = default;

    virtual void StartPixelSample(const Point2i& pixel, int32 sample_index);

    virtual Float Next1D() = 0;
    virtual Point2 Next2D() = 0;

    virtual std::unique_ptr<Sampler> Clone() const = 0;

    const int32 samples_per_pixel;

protected:
    Point2i current_pixel;
    int32 current_sample_index;
};

inline Sampler::Sampler(int32 spp)
    : samples_per_pixel{ spp }
{
}

inline void Sampler::StartPixelSample(const Point2i& pixel, int32 sample_index)
{
    current_pixel = pixel;
    current_sample_index = sample_index;
}

} // namespace bulbit
