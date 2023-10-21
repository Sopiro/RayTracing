#pragma once

#include "scene.h"

namespace bulbit
{

class Integrator
{
public:
    virtual ~Integrator() = default;
    virtual Spectrum Li(const Scene& scene, const Ray& ray) const
    {
        return Spectrum::black;
    }
};

} // namespace bulbit
