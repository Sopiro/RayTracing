#pragma once

#include "camera.h"
#include "film.h"
#include "light_sampler.h"
#include "sampler.h"
#include "scene.h"

namespace bulbit
{

class Integrator
{
public:
    virtual ~Integrator() = default;

    virtual void Render(Film* film, const Camera& camera) = 0;

protected:
    Integrator(const Intersectable* accel, std::vector<Light*> lights)
        : accel{ accel }
        , all_lights{ std::move(lights) }
    {
    }

    bool Intersect(Intersection* out_isect, const Ray& ray, Float t_min, Float t_max) const
    {
        return accel->Intersect(out_isect, ray, t_min, t_max);
    }

    bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const
    {
        return accel->IntersectAny(ray, t_min, t_max);
    }

    const Intersectable* accel;
    std::vector<Light*> all_lights;
};

class SamplerIntegrator : public Integrator
{
public:
    SamplerIntegrator(const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler);
    virtual ~SamplerIntegrator() = default;

    virtual Spectrum Li(const Ray& ray, Sampler& sampler) const = 0;

    virtual void Render(Film* film, const Camera& camera) override;

private:
    const Sampler* sampler_prototype;
};

class DebugIntegrator : public SamplerIntegrator
{
public:
    DebugIntegrator(const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler);
    virtual ~DebugIntegrator() = default;

    virtual Spectrum Li(const Ray& ray, Sampler& sampler) const override;
};

// Whitted-style raytracing
class WhittedStyle : public SamplerIntegrator
{
public:
    WhittedStyle(const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler, int32 max_depth);
    virtual ~WhittedStyle() = default;

    virtual Spectrum Li(const Ray& ray, Sampler& sampler) const override
    {
        return Li(ray, sampler, 0);
    }

private:
    Spectrum Li(const Ray& ray, Sampler& sampler, int32 depth) const;

    std::vector<Light*> infinite_lights;

    int32 max_depth;
};

// This integrator evaluates ambient occlusion
class AmbientOcclusion : public SamplerIntegrator
{
public:
    AmbientOcclusion(const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler, Float ao_range);
    virtual ~AmbientOcclusion() = default;

    virtual Spectrum Li(const Ray& ray, Sampler& sampler) const override;

private:
    // maximum range to consider occlusuion
    Float range;
};

class AlbedoIntegrator : public SamplerIntegrator
{
public:
    AlbedoIntegrator(const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler);
    virtual ~AlbedoIntegrator() = default;

    virtual Spectrum Li(const Ray& ray, Sampler& sampler) const override;

private:
    std::vector<Light*> infinite_lights;
};

class NaivePathIntegrator : public SamplerIntegrator
{
public:
    NaivePathIntegrator(const Intersectable* accel,
                        std::vector<Light*> lights,
                        const Sampler* sampler,
                        int32 max_bounces,
                        Float russian_roulette_probability = 0.95f);
    virtual ~NaivePathIntegrator() = default;

    virtual Spectrum Li(const Ray& ray, Sampler& sampler) const override;

private:
    Spectrum Li(const Ray& ray, Sampler& sampler, int32 depth) const;

    std::vector<Light*> infinite_lights;

    int32 max_bounces;
    Float rr_probability;
};

// Uni-directional path tracer
class PathIntegrator : public SamplerIntegrator
{
public:
    PathIntegrator(const Intersectable* accel,
                   std::vector<Light*> lights,
                   const Sampler* sampler,
                   int32 max_bounces,
                   bool regularize_bsdf = false,
                   Float russian_roulette_probability = 0.95f);
    virtual ~PathIntegrator() = default;

    virtual Spectrum Li(const Ray& ray, Sampler& sampler) const override;

private:
    std::vector<Light*> infinite_lights;
    std::unordered_map<const Primitive*, AreaLight*> area_lights;
    UniformLightSampler light_sampler;

    int32 max_bounces;
    Float rr_probability;
    bool regularize_bsdf;
};

} // namespace bulbit
