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
    NaivePathIntegrator(const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler, int32 max_bounces);
    virtual ~NaivePathIntegrator() = default;

    virtual Spectrum Li(const Ray& ray, Sampler& sampler) const override;

private:
    Spectrum Li(const Ray& ray, Sampler& sampler, int32 depth) const;

    std::vector<Light*> infinite_lights;

    int32 max_bounces;
};

// Uni-directional path tracer
class PathIntegrator : public SamplerIntegrator
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

    virtual Spectrum Li(const Ray& ray, Sampler& sampler) const override;

private:
    Spectrum SampleDirectLight(const Vec3& wo, const Intersection& isect, BSDF* bsdf, Sampler& sampler, const Spectrum& beta)
        const;

    std::vector<Light*> infinite_lights;
    std::unordered_map<const Primitive*, AreaLight*> area_lights;
    UniformLightSampler light_sampler;

    int32 max_bounces;
    bool regularize_bsdf;
};

class NaiveVolPathIntegrator : public SamplerIntegrator
{
public:
    NaiveVolPathIntegrator(const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler, int32 max_bounces);
    virtual ~NaiveVolPathIntegrator() = default;

    virtual Spectrum Li(const Ray& ray, Sampler& sampler) const override;

private:
    std::vector<Light*> infinite_lights;

    int32 max_bounces;
};

class VolPathIntegrator : public SamplerIntegrator
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

    virtual Spectrum Li(const Ray& ray, Sampler& sampler) const override;

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
