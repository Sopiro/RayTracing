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

class UniDirectionalRayIntegrator : public Integrator
{
public:
    UniDirectionalRayIntegrator(const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler);
    virtual ~UniDirectionalRayIntegrator() = default;

    virtual void Render(Film* film, const Camera& camera) override;

    virtual Spectrum Li(const Ray& ray, const Medium* medium, Sampler& sampler) const = 0;

private:
    const Sampler* sampler_prototype;
};

} // namespace bulbit
