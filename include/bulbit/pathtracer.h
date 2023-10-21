#pragma once

#include "integrator.h"

namespace bulbit
{

class PathTracer : public Integrator
{
public:
    PathTracer(int32 max_bounces, Float russian_roulette_probability = Float(0.95));
    virtual ~PathTracer() = default;

    virtual Spectrum Li(const Scene& scene, const Ray& ray) const override;

private:
    int32 max_bounces;
    Float rr_probability;
};

} // namespace bulbit
