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

inline IndependentSampler::IndependentSampler(int32 samples_per_pixel, int32 seed)
    : Sampler(samples_per_pixel)
{
    rng.Seed(seed);
}

inline void IndependentSampler::StartPixelSample(const Point2i& pixel, int32 sample_index)
{
    Sampler::StartPixelSample(pixel, sample_index);
    rng.Seed(Hash(pixel, seed, sample_index));
}

inline Float IndependentSampler::Next1D()
{
    return rng.NextFloat();
}

inline Point2 IndependentSampler::Next2D()
{
    return Point2{ rng.NextFloat(), rng.NextFloat() };
}

inline std::unique_ptr<Sampler> IndependentSampler::Clone() const
{
    return std::make_unique<IndependentSampler>(samples_per_pixel, seed);
}

} // namespace bulbit
