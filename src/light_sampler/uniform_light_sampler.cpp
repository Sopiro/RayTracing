#include "bulbit/light_sampler.h"

namespace bulbit
{

UniformLightSampler::UniformLightSampler(const std::vector<Light*>& lights)
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

Float UniformLightSampler::EvaluatePMF(Light* light) const
{
    if (lights.size() > 0)
    {
        return 1.0f / lights.size();
    }
    else
    {
        return 0;
    }
}

} // namespace bulbit
