#pragma once

#include "random.h"
#include "sampler.h"

namespace bulbit
{

class IndependentSampler : public Sampler
{
public:
    IndependentSampler(int32 samples_per_pixel, int32 seed = 0);
    virtual ~IndependentSampler() = default;

    virtual void StartPixelSample(const Point2i& pixel, int32 sample_index) override;

    virtual Float Next1D() override;
    virtual Point2 Next2D() override;

    virtual std::unique_ptr<Sampler> Clone() const override;

private:
    int32 seed;
    RNG rng;
};

class StratifiedSampler : public Sampler
{
public:
    StratifiedSampler(int32 x_samples, int32 y_samples, bool jitter, int32 seed = 0);
    virtual void StartPixelSample(const Point2i& pixel, int32 sample_index) override;

    virtual Float Next1D() override;
    virtual Point2 Next2D() override;

    virtual std::unique_ptr<Sampler> Clone() const override;

private:
    bool jitter;
    int32 dimension;

    int32 x_samples, y_samples;

    int32 seed;
    RNG rng;
};

} // namespace bulbit
