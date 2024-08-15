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
    const Float uc_rho[rho_samples] = { 0.75741637, 0.37870818, 0.7083487, 0.18935409, 0.9149363, 0.35417435,
                                        0.5990858,  0.09467703, 0.8578725, 0.45746812, 0.686759,  0.17708716,
                                        0.9674518,  0.2995429,  0.5083201, 0.047338516 };
    const Point2 u_rho[rho_samples] = { Point2(0.855985, 0.570367), Point2(0.381823, 0.851844), Point2(0.285328, 0.764262),
                                        Point2(0.733380, 0.114073), Point2(0.542663, 0.344465), Point2(0.127274, 0.414848),
                                        Point2(0.964700, 0.947162), Point2(0.594089, 0.643463), Point2(0.095109, 0.170369),
                                        Point2(0.825444, 0.263359), Point2(0.429467, 0.454469), Point2(0.244460, 0.816459),
                                        Point2(0.756135, 0.731258), Point2(0.516165, 0.152852), Point2(0.180888, 0.214174),
                                        Point2(0.898579, 0.503897) };

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
