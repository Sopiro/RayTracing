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
    LightSampler(const std::vector<Light*>& lights)
        : lights{ lights }
    {
    }

    virtual ~LightSampler() = default;

    virtual bool Sample(SampledLight* sampled_light, const Intersection& is, Float u) const = 0;
    virtual Float EvaluatePMF(Light* light) const = 0;

protected:
    const std::vector<Light*>& lights;
};

class UniformLightSampler : public LightSampler
{
public:
    UniformLightSampler(const std::vector<Light*>& lights);
    virtual ~UniformLightSampler() = default;

    virtual bool Sample(SampledLight* sampled_light, const Intersection& is, Float u) const override;
    virtual Float EvaluatePMF(Light* light) const override;
};

} // namespace bulbit
