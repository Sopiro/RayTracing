#pragma once

#include "integrator.h"
#include "light_samplers.h"
#include "lights.h"

namespace bulbit
{

class DebugIntegrator : public UniDirectionalRayIntegrator
{
public:
    DebugIntegrator(const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler);
    virtual ~DebugIntegrator() = default;

    virtual Spectrum Li(const Ray& ray, const Medium* medium, Sampler& sampler) const override;
};

// This integrator evaluates ambient occlusion
class AmbientOcclusion : public UniDirectionalRayIntegrator
{
public:
    AmbientOcclusion(const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler, Float ao_range);
    virtual ~AmbientOcclusion() = default;

    virtual Spectrum Li(const Ray& ray, const Medium* medium, Sampler& sampler) const override;

private:
    // maximum range to consider occlusuion
    Float range;
};

class AlbedoIntegrator : public UniDirectionalRayIntegrator
{
public:
    AlbedoIntegrator(const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler);
    virtual ~AlbedoIntegrator() = default;

    virtual Spectrum Li(const Ray& ray, const Medium* medium, Sampler& sampler) const override;

private:
    std::vector<Light*> infinite_lights;
};

// Whitted-style raytracing
class WhittedStyle : public UniDirectionalRayIntegrator
{
public:
    WhittedStyle(const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler, int32 max_depth);
    virtual ~WhittedStyle() = default;

    virtual Spectrum Li(const Ray& ray, const Medium* medium, Sampler& sampler) const override
    {
        return Li(ray, sampler, 0);
    }

private:
    Spectrum Li(const Ray& ray, Sampler& sampler, int32 depth) const;

    std::vector<Light*> infinite_lights;

    int32 max_depth;
};

class NaivePathIntegrator : public UniDirectionalRayIntegrator
{
public:
    NaivePathIntegrator(const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler, int32 max_bounces);
    virtual ~NaivePathIntegrator() = default;

    virtual Spectrum Li(const Ray& ray, const Medium* medium, Sampler& sampler) const override;

private:
    Spectrum Li(const Ray& ray, Sampler& sampler, int32 depth) const;

    std::vector<Light*> infinite_lights;

    int32 max_bounces;
};

// Uni-directional path tracer
class PathIntegrator : public UniDirectionalRayIntegrator
{
public:
    PathIntegrator(
        const Intersectable* accel,
        std::vector<Light*> lights,
        const Sampler* sampler,
        int32 max_bounces,
        bool regularize_bsdf = false
    );
    virtual ~PathIntegrator() = default;

    virtual Spectrum Li(const Ray& ray, const Medium* medium, Sampler& sampler) const override;

private:
    Spectrum SampleDirectLight(const Vec3& wo, const Intersection& isect, BSDF* bsdf, Sampler& sampler, const Spectrum& beta)
        const;

    std::vector<Light*> infinite_lights;
    std::unordered_map<const Primitive*, AreaLight*> area_lights;
    UniformLightSampler light_sampler;

    int32 max_bounces;
    bool regularize_bsdf;
};

class NaiveVolPathIntegrator : public UniDirectionalRayIntegrator
{
public:
    NaiveVolPathIntegrator(const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler, int32 max_bounces);
    virtual ~NaiveVolPathIntegrator() = default;

    virtual Spectrum Li(const Ray& ray, const Medium* medium, Sampler& sampler) const override;

private:
    std::vector<Light*> infinite_lights;

    int32 max_bounces;
};

class VolPathIntegrator : public UniDirectionalRayIntegrator
{
public:
    VolPathIntegrator(
        const Intersectable* accel,
        std::vector<Light*> lights,
        const Sampler* sampler,
        int32 max_bounces,
        bool regularize_bsdf = false
    );
    virtual ~VolPathIntegrator() = default;

    virtual Spectrum Li(const Ray& ray, const Medium* medium, Sampler& sampler) const override;

private:
    Spectrum SampleDirectLight(
        const Vec3& wo,
        const Intersection& isect,
        const Medium* medium,
        const BSDF* bsdf,
        const PhaseFunction* phase,
        int32 wavelength,
        Sampler& sampler,
        Spectrum beta,
        Spectrum r_p
    ) const;

    std::vector<Light*> infinite_lights;
    std::unordered_map<const Primitive*, AreaLight*> area_lights;
    UniformLightSampler light_sampler;

    int32 max_bounces;
    bool regularize_bsdf;
};

} // namespace bulbit
