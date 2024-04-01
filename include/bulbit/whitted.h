#pragma once

#include "integrator.h"

namespace bulbit
{

// Whitted-style raytracing
class WhittedStyle : public SamplerIntegrator
{
public:
    WhittedStyle(const Ref<Sampler> sampler, int32 max_depth);
    virtual ~WhittedStyle() = default;

    virtual Spectrum Li(const Scene& scene, const Ray& ray, Sampler& sampler) const override;

private:
    Spectrum Li(const Scene& scene, const Ray& ray, Sampler& sampler, int32 depth) const;

    int32 max_depth;
};

inline Spectrum WhittedStyle::Li(const Scene& scene, const Ray& primary_ray, Sampler& sampler) const
{
    return Li(scene, primary_ray, sampler, 0);
}

} // namespace bulbit
