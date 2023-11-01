#pragma once

#include "camera.h"
#include "film.h"
#include "sampler.h"
#include "scene.h"

namespace bulbit
{

class Integrator
{
public:
    virtual ~Integrator() = default;

    virtual void Render(Film* film, const Scene& scene, const Camera& camera) = 0;
};

class SamplerIntegrator : public Integrator
{
public:
    SamplerIntegrator(const Ref<Sampler> sampler);
    virtual ~SamplerIntegrator() = default;

    virtual Spectrum Li(const Scene& scene, const Ray& ray, Sampler& sampler) const = 0;

    virtual void Preprocess(const Scene& scene, const Camera& camera){};
    virtual void Render(Film* film, const Scene& scene, const Camera& camera) override;

private:
    Ref<Sampler> sampler;
};

} // namespace bulbit
