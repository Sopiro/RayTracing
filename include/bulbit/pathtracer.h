#pragma once

#include "integrator.h"

namespace bulbit
{

class PathTracer : public SamplerIntegrator
{
public:
    PathTracer(const Ref<Sampler> sampler, int32 max_bounces, Float russian_roulette_probability = Float(0.95));
    virtual ~PathTracer() = default;

    virtual Spectrum Li(const Scene& scene, const Ray& ray, Sampler& sampler) const override;

private:
    int32 max_bounces;
    Float rr_probability;
};

} // namespace bulbit
