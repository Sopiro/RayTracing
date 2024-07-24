#pragma once

#include "common.h"
#include "light.h"

namespace bulbit
{

struct SampledLight
{
    const Light* light;
    Float weight;
};

class LightSampler
{
public:
    LightSampler(std::span<Light*> lights)
        : lights{ lights }
    {
    }

    virtual ~LightSampler() = default;

    virtual bool Sample(SampledLight* sampled_light, const Intersection& isect, Float u) const = 0;
    virtual Float EvaluatePMF(const Light* light) const = 0;

protected:
    std::span<Light*> lights;
};

} // namespace bulbit
