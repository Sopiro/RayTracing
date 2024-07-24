#include "bulbit/light_samplers.h"

namespace bulbit
{

UniformLightSampler::UniformLightSampler(std::span<Light*> lights)
    : LightSampler(lights)
{
}

bool UniformLightSampler::Sample(SampledLight* sl, const Intersection& isect, Float u) const
{
    size_t count = lights.size();
    if (count == 0)
    {
        return false;
    }

    size_t index = std::min(size_t(u * count), count - 1);

    sl->light = lights[index];
    sl->weight = count;

    return true;
}

Float UniformLightSampler::EvaluatePMF(const Light* light) const
{
    if (lights.size() > 0)
    {
        return Float(1) / lights.size();
    }
    else
    {
        return 0;
    }
}

} // namespace bulbit
