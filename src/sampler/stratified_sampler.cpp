#include "bulbit/hash.h"
#include "bulbit/samplers.h"

namespace bulbit
{

StratifiedSampler::StratifiedSampler(int32 x_samples, int32 y_samples, bool jitter, int32 seed)
    : Sampler(x_samples * y_samples)
    , jitter(jitter)
    , x_samples(x_samples)
    , y_samples(y_samples)
    , seed(seed)
{
}

void StratifiedSampler::StartPixelSample(const Point2i& pixel, int32 sample_index)
{
    Sampler::StartPixelSample(pixel, sample_index);
    rng.Seed(Hash(pixel, seed, sample_index));

    dimension = 0;
}

Float StratifiedSampler::Next1D()
{
    uint64_t hash = Hash(seed, current_pixel, dimension);
    int32 stratum = PermutationElement(current_sample_index, samples_per_pixel, hash);

    dimension += 1;

    Float delta = jitter ? rng.NextFloat() : 0.5f;

    return (stratum + delta) / samples_per_pixel;
}

Point2 StratifiedSampler::Next2D()
{
    uint64_t hash = Hash(seed, current_pixel, dimension);
    int32 stratum = PermutationElement(current_sample_index, samples_per_pixel, hash);

    dimension += 2;

    int32 x = stratum % x_samples;
    int32 y = stratum / x_samples;

    Float dx = jitter ? rng.NextFloat() : 0.5f;
    Float dy = jitter ? rng.NextFloat() : 0.5f;

    return { (x + dx) / x_samples, (y + dy) / y_samples };
}

std::unique_ptr<Sampler> StratifiedSampler::Clone() const
{
    return std::make_unique<StratifiedSampler>(x_samples, y_samples, jitter, seed);
}

} // namespace bulbit
