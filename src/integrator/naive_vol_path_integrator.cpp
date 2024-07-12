#include "bulbit/bxdfs.h"
#include "bulbit/integrator.h"
#include "bulbit/material.h"
#include "bulbit/media.h"
#include "bulbit/random.h"

namespace bulbit
{

NaiveVolPathIntegrator::NaiveVolPathIntegrator(
    const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler, int32 max_bounces, Float rr_probability)
    : SamplerIntegrator(accel, std::move(lights), sampler)
    , max_bounces{ max_bounces }
    , rr_probability{ rr_probability }
{
    for (Light* light : all_lights)
    {
        switch (light->type)
        {
        case Light::Type::infinite_light:
        {
            infinite_lights.push_back(light);
        }
        break;
        case Light::Type::area_light:
        {
            AreaLight* area_light = (AreaLight*)light;
            area_lights.emplace(area_light->GetPrimitive(), area_light);
        }
        break;
        default:
            break;
        }
    }
}

Spectrum NaiveVolPathIntegrator::Li(const Ray& primary_ray, Sampler& sampler) const
{
    int32 bounce = 0;
    Spectrum L(0), throughput(1);
    Float eta_scale = 1;
    Ray ray = primary_ray;
    const Medium* medium = nullptr;

    while (true)
    {
        bool scattered = false;
        bool terminated = false;

        Intersection isect;
        bool found_intersection = Intersect(&isect, ray, Ray::epsilon, infinity);

        if (medium)
        {
            uint64 hash0 = Hash(sampler.Next1D());
            uint64 hash1 = Hash(sampler.Next1D());
            RNG rng(hash0, hash1);

            Float t_max = found_intersection ? isect.t : infinity;
            Float u = sampler.Next1D();
            Float u_event = sampler.Next1D();

            Sample_MajorantTransmittance(
                medium, ray, t_max, u, rng, [&](Point3 p, MediumSample ms, Spectrum sigma_maj, Spectrum T_maj) -> bool {
                    Float p_absorb = ms.sigma_a[0] / sigma_maj[0];
                    Float p_scatter = ms.sigma_s[0] / sigma_maj[0];
                    Float p_null = std::max<Float>(0, 1 - p_absorb - p_scatter);
                    Float events[3] = { p_absorb, p_scatter, p_null };

                    int32 event = SampleDiscrete(events, u_event);
                    switch (event)
                    {
                    case 0:
                    {
                        // Sample absorption event, incorporate emission and terminate path
                        L += throughput * ms.Le;
                        terminated = true;

                        return false;
                    }

                    case 1:
                    {
                        // Sample real scattering event
                        if (bounce++ >= max_bounces)
                        {
                            terminated = true;
                            return false;
                        }

                        // Sample phase function for real scattering event
                        Point2 u{ rng.NextFloat(), rng.NextFloat() };
                        PhaseFunctionSample ps;
                        if (!ms.phase->Sample_p(&ps, -ray.d, u))
                        {
                            terminated = true;
                            return false;
                        }

                        throughput *= ps.p / ps.pdf;
                        ray.o = p;
                        ray.d = ps.wi;
                        scattered = true;

                        return false;
                    }

                    case 2:
                    {
                        // Sample null scattering event, continue sampling
                        u_event = rng.NextFloat();
                        return true;
                    }

                    default:
                        assert(false);
                        return false;
                    }
                });
        }

        if (terminated)
        {
            return L;
        }
        if (scattered)
        {
            continue;
        }

        if (!found_intersection)
        {
            for (Light* light : infinite_lights)
            {
                L += throughput * light->Le(ray);
            }

            break;
        }

        if (bounce++ >= max_bounces)
        {
            break;
        }

        Vec3 wo = Normalize(-ray.d);

        // Incorporate surface emission
        L += throughput * isect.Le(wo);

        int8 mem[max_bxdf_size];
        Resource res(mem, sizeof(mem));
        Allocator alloc(&res);
        BSDF bsdf;
        if (!isect.GetBSDF(&bsdf, wo, alloc))
        {
            medium = isect.GetMedium(-wo);
            ray = Ray(isect.point, -wo);
            continue;
        }

        BSDFSample bsdf_sample;
        if (!bsdf.Sample_f(&bsdf_sample, wo, sampler.Next1D(), sampler.Next2D()))
        {
            break;
        }

        if (bsdf_sample.IsTransmission())
        {
            eta_scale *= Sqr(bsdf_sample.eta);
        }

        throughput *= bsdf_sample.f * AbsDot(isect.shading.normal, bsdf_sample.wi) / bsdf_sample.pdf;
        medium = isect.GetMedium(bsdf_sample.wi);
        ray = Ray(isect.point, bsdf_sample.wi);

        // Terminate path with russian roulette
        constexpr int32 min_bounces = 2;
        if (bounce > min_bounces)
        {
            Float rr = std::fmin(rr_probability, throughput.Luminance() * eta_scale);
            if (sampler.Next1D() > rr)
            {
                break;
            }

            throughput *= 1 / rr;
        }
    }

    return L;
}

} // namespace bulbit
