#pragma once

#include "integrator.h"

namespace bulbit
{

// Uni-directional path tracer
class PathIntegrator : public SamplerIntegrator
{
public:
    PathIntegrator(const Ref<Sampler> sampler, int32 max_bounces, Float russian_roulette_probability = Float(0.95));
    virtual ~PathIntegrator() = default;

    virtual Spectrum Li(const Scene& scene, const Ray& ray, Sampler& sampler) const override;

private:
    int32 max_bounces;
    Float rr_probability;
};

class NaivePathIntegrator : public SamplerIntegrator
{
public:
    NaivePathIntegrator(const Ref<Sampler> sampler, int32 max_bounces, Float russian_roulette_probability = Float(0.95));
    virtual ~NaivePathIntegrator() = default;

    virtual Spectrum Li(const Scene& scene, const Ray& ray, Sampler& sampler) const override;

private:
    Spectrum Li(const Scene& scene, const Ray& ray, Sampler& sampler, int32 depth) const;

    int32 max_bounces;
    Float rr_probability;
};

} // namespace bulbit
