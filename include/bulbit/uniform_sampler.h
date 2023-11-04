#pragma once

#include "random.h"
#include "sampler.h"

namespace bulbit
{

class UniformSampler : public Sampler
{
public:
    UniformSampler(int32 samples_per_pixel, int32 seed = 0);
    virtual ~UniformSampler() = default;

    virtual Float Next1D() override;
    virtual Point2 Next2D() override;

    virtual std::unique_ptr<Sampler> Clone(int32 seed) override;

private:
    RNG rng;
};

inline UniformSampler::UniformSampler(int32 samples_per_pixel, int32 seed)
    : Sampler(samples_per_pixel)
{
    rng.Seed(seed);
}

inline Float UniformSampler::Next1D()
{
    return rng.NextFloat();
}

inline Point2 UniformSampler::Next2D()
{
    return { rng.NextFloat(), rng.NextFloat() };
}

inline std::unique_ptr<Sampler> UniformSampler::Clone(int32 seed)
{
    return std::make_unique<UniformSampler>(samples_per_pixel, seed);
}

} // namespace bulbit
