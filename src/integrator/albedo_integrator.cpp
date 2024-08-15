#include "bulbit/bxdfs.h"
#include "bulbit/frame.h"
#include "bulbit/integrators.h"

namespace bulbit
{

AlbedoIntegrator::AlbedoIntegrator(const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler)
    : UniDirectionalRayIntegrator(accel, std::move(lights), sampler)
{
    for (Light* light : all_lights)
    {
        if (light->Is<ImageInfiniteLight>() || light->Is<UniformInfiniteLight>())
        {
            infinite_lights.push_back(light);
        }
    }
}

Spectrum AlbedoIntegrator::Li(const Ray& ray, const Medium* medium, Sampler& sampler) const
{
    BulbitNotUsed(medium);
    BulbitNotUsed(sampler);

    Spectrum L(0);

    Intersection isect;
    if (!Intersect(&isect, ray, Ray::epsilon, infinity))
    {
        for (Light* light : infinite_lights)
        {
            L += light->Le(ray);
        }

        return L;
    }

    // Precomputed Halton samples from pbrt4 code
    constexpr int rho_samples = 16;
    const Float uc_rho[rho_samples] = { 0.75741637f, 0.37870818f, 0.7083487f, 0.18935409f, 0.9149363f, 0.35417435f,
                                        0.5990858f,  0.09467703f, 0.8578725f, 0.45746812f, 0.686759f,  0.17708716f,
                                        0.9674518f,  0.2995429f,  0.5083201f, 0.047338516f };
    const Point2 u_rho[rho_samples] = { Point2(0.855985f, 0.570367f), Point2(0.381823f, 0.851844f), Point2(0.285328f, 0.764262f),
                                        Point2(0.733380f, 0.114073f), Point2(0.542663f, 0.344465f), Point2(0.127274f, 0.414848f),
                                        Point2(0.964700f, 0.947162f), Point2(0.594089f, 0.643463f), Point2(0.095109f, 0.170369f),
                                        Point2(0.825444f, 0.263359f), Point2(0.429467f, 0.454469f), Point2(0.244460f, 0.816459f),
                                        Point2(0.756135f, 0.731258f), Point2(0.516165f, 0.152852f), Point2(0.180888f, 0.214174f),
                                        Point2(0.898579f, 0.503897f) };

    Vec3 wo = Normalize(-ray.d);

    L += isect.Le(wo);

    int8 mem[max_bxdf_size];
    Resource res(mem, sizeof(mem));
    Allocator alloc(&res);
    BSDF bsdf;
    if (!isect.GetBSDF(&bsdf, wo, alloc))
    {
        return L;
    }

    L += bsdf.rho(wo, uc_rho, u_rho);

    return L;
}

} // namespace bulbit
