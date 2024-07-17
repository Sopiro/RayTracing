#include "bulbit/bxdfs.h"
#include "bulbit/integrator.h"
#include "bulbit/material.h"
#include "bulbit/media.h"
#include "bulbit/random.h"

namespace bulbit
{

NaiveVolPathIntegrator::NaiveVolPathIntegrator(
    const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler, int32 max_bounces
)
    : SamplerIntegrator(accel, std::move(lights), sampler)
    , max_bounces{ max_bounces }
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
        }
        break;
        default:
            assert(false);
            break;
        }
    }
}

Spectrum NaiveVolPathIntegrator::Li(const Ray& primary_ray, Sampler& sampler) const
{
    int32 bounce = 0;
    Spectrum L(0), throughput(1);
    Spectrum r_u(1); // Rescaled path sampling probability
    Float eta_scale = 1;

    Ray ray = primary_ray;
    int32 wavelength = std::min<int32>(2, sampler.Next1D() * 3);

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

            // Evaluate L_n term which is the null-scattering extended source function by delta tracking
            Spectrum T_maj = Sample_MajorantTransmittance(
                medium, wavelength, ray, t_max, u, rng,
                [&](Point3 p, MediumSample ms, Spectrum sigma_maj, Spectrum T_maj) -> bool {
                    Float p_absorb = ms.sigma_a[wavelength] / sigma_maj[wavelength];
                    Float p_scatter = ms.sigma_s[wavelength] / sigma_maj[wavelength];
                    Float p_null = std::max<Float>(0, 1 - p_absorb - p_scatter);
                    Float events[3] = { p_absorb, p_scatter, p_null };

                    int32 event = SampleDiscrete(events, u_event);
                    switch (event)
                    {
                    case 0:
                    {
                        // Sampled absorption event, incorporate emission and terminate path
                        Float pdf = T_maj[wavelength] * ms.sigma_a[wavelength];
                        throughput *= T_maj / pdf;
                        r_u *= T_maj * ms.sigma_a / pdf;

                        L += throughput * ms.sigma_a * ms.Le / r_u.Average();
                        terminated = true;

                        return false;
                    }

                    case 1:
                    {
                        // Sampled real scattering event
                        if (bounce++ >= max_bounces)
                        {
                            terminated = true;
                            return false;
                        }

                        // Sample phase function to find next path direction
                        Point2 u{ rng.NextFloat(), rng.NextFloat() };
                        PhaseFunctionSample ps;
                        if (!ms.phase->Sample_p(&ps, -ray.d, u))
                        {
                            terminated = true;
                            return false;
                        }

                        Float pdf = T_maj[wavelength] * ms.sigma_s[wavelength];
                        throughput *= T_maj * ms.sigma_s / pdf;
                        r_u *= T_maj * ms.sigma_s / pdf;

                        throughput *= ps.p / ps.pdf;
                        ray.o = p;
                        ray.d = ps.wi;
                        scattered = true;

                        return false;
                    }

                    case 2:
                    {
                        // Sampled null scattering event, continue sampling
                        u_event = rng.NextFloat();
                        Spectrum sigma_n = Max(sigma_maj - ms.sigma_a - ms.sigma_s, 0);

                        Float pdf = T_maj[wavelength] * sigma_n[wavelength];
                        throughput *= T_maj * sigma_n / pdf;
                        if (pdf == 0) throughput = Spectrum::black;
                        r_u *= T_maj * sigma_n / pdf;

                        return !throughput.IsBlack() && !r_u.IsBlack();
                    }

                    default:
                        assert(false);
                        return false;
                    }
                }
            );

            throughput *= T_maj / T_maj[wavelength];
            r_u *= T_maj / T_maj[wavelength];
        }

        if (terminated)
        {
            break;
        }
        if (scattered)
        {
            continue;
        }

        Vec3 wo = Normalize(-ray.d);

        if (!found_intersection)
        {
            for (Light* light : infinite_lights)
            {
                L += throughput * light->Le(ray) / r_u.Average();
            }

            break;
        }

        // Add surface emission
        if (Spectrum Le = isect.Le(wo); !Le.IsBlack())
        {
            L += throughput * Le / r_u.Average();
        }

        if (bounce++ >= max_bounces)
        {
            break;
        }

        int8 mem[max_bxdf_size];
        Resource res(mem, sizeof(mem));
        Allocator alloc(&res);
        BSDF bsdf;
        if (!isect.GetBSDF(&bsdf, wo, alloc))
        {
            medium = isect.GetMedium(-wo);
            ray = Ray(isect.point, -wo);
            --bounce;
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
            Float rr = throughput.MaxComponent() * eta_scale / r_u.Average();
            if (rr < 1 && sampler.Next1D() > rr)
            {
                break;
            }

            throughput /= rr;
        }
    }

    return L;
}

} // namespace bulbit
