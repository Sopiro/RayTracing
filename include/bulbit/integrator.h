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
    SamplerIntegrator(const std::shared_ptr<Sampler> sampler);
    virtual ~SamplerIntegrator() = default;

    virtual Spectrum Li(const Scene& scene, const Ray& ray, Sampler& sampler) const = 0;

    virtual void Preprocess(const Scene& scene, const Camera& camera){};
    virtual void Render(Film* film, const Scene& scene, const Camera& camera) override;

private:
    std::shared_ptr<Sampler> sampler_prototype;
};

class DebugIntegrator : public SamplerIntegrator
{
public:
    DebugIntegrator(const std::shared_ptr<Sampler> sampler);
    virtual ~DebugIntegrator() = default;

    virtual Spectrum Li(const Scene& scene, const Ray& ray, Sampler& sampler) const override;
};

// Whitted-style raytracing
class WhittedStyle : public SamplerIntegrator
{
public:
    WhittedStyle(const std::shared_ptr<Sampler> sampler, int32 max_depth);
    virtual ~WhittedStyle() = default;

    virtual Spectrum Li(const Scene& scene, const Ray& ray, Sampler& sampler) const override
    {
        return Li(scene, ray, sampler, 0);
    }

private:
    Spectrum Li(const Scene& scene, const Ray& ray, Sampler& sampler, int32 depth) const;

    int32 max_depth;
};

// This integrator evaluates ambient occlusion
class AmbientOcclusion : public SamplerIntegrator
{
public:
    AmbientOcclusion(const std::shared_ptr<Sampler> sampler, Float ao_range);
    virtual ~AmbientOcclusion() = default;

    virtual Spectrum Li(const Scene& scene, const Ray& ray, Sampler& sampler) const override;

private:
    // maximum range to consider occlusuion
    Float range;
};

class NaivePathIntegrator : public SamplerIntegrator
{
public:
    NaivePathIntegrator(const std::shared_ptr<Sampler> sampler,
                        int32 max_bounces,
                        Float russian_roulette_probability = Float(0.95));
    virtual ~NaivePathIntegrator() = default;

    virtual Spectrum Li(const Scene& scene, const Ray& ray, Sampler& sampler) const override
    {
        return Li(scene, ray, sampler, 0);
    }

private:
    Spectrum Li(const Scene& scene, const Ray& ray, Sampler& sampler, int32 depth) const;

    int32 max_bounces;
    Float rr_probability;
};

// Uni-directional path tracer
class PathIntegrator : public SamplerIntegrator
{
public:
    PathIntegrator(const std::shared_ptr<Sampler> sampler, int32 max_bounces, Float russian_roulette_probability = Float(0.95));
    virtual ~PathIntegrator() = default;

    virtual Spectrum Li(const Scene& scene, const Ray& ray, Sampler& sampler) const override;

private:
    int32 max_bounces;
    Float rr_probability;
};

} // namespace bulbit
