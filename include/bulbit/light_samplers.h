#pragma once

#include "light_sampler.h"

namespace bulbit
{

class UniformLightSampler : public LightSampler
{
public:
    UniformLightSampler(std::span<Light*> lights);
    virtual ~UniformLightSampler() = default;

    virtual bool Sample(SampledLight* sampled_light, const Intersection& isect, Float u) const override;
    virtual Float EvaluatePMF(const Light* light) const override;
};

} // namespace bulbit
