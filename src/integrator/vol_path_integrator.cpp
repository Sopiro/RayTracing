#include "bulbit/bxdfs.h"
#include "bulbit/integrators.h"
#include "bulbit/material.h"
#include "bulbit/media.h"
#include "bulbit/random.h"

namespace bulbit
{

VolPathIntegrator::VolPathIntegrator(
    const Intersectable* accel, std::vector<Light*> lights, const Sampler* sampler, int32 max_bounces, bool regularize_bsdf
)
    : UniDirectionalRayIntegrator(accel, std::move(lights), sampler)
    , max_bounces{ max_bounces }
    , regularize_bsdf{ regularize_bsdf }
    , light_sampler{ all_lights }
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

Spectrum VolPathIntegrator::Li(const Ray& primary_ray, Sampler& sampler) const
{
    int32 wavelength = std::min<int32>(sampler.Next1D() * 3, 2);
    int32 bounce = 0;
    Spectrum L(0), beta(1);

    // Wavelength dependent rescaled path sampling probabilities
    Spectrum r_u(1); // Indirect path sampling pdf
    Spectrum r_l(1); // Light path sampling pdf

    bool specular_bounce = false;
    bool any_non_specular_bounces = false;

    Float eta_scale = 1;
    Ray ray = primary_ray;

    const Medium* medium = nullptr;

    Point3 last_scattering_vertex;

    while (true)
    {
        Vec3 wo = Normalize(-ray.d);
        Intersection isect;
        bool found_intersection = Intersect(&isect, ray, Ray::epsilon, infinity);

        if (medium)
        {
            bool scattered = false;
            bool terminated = false;

            Float t_max = found_intersection ? isect.t : infinity;
            Float u = sampler.Next1D();

            uint64 hash0 = Hash(sampler.Next1D());
            uint64 hash1 = Hash(sampler.Next1D());
            RNG rng(hash0, hash1);

            // Sample the participating medium
            // If the sampled point is inside the extent, evaluate the L_n term
            // otherwise evaluate the L_o term
            Spectrum T_maj = Sample_MajorantTransmittance(
                medium, wavelength, ray, t_max, u, rng,
                [&](Point3 p, MediumSample ms, Spectrum sigma_maj, Spectrum T_maj) -> bool {
                    if (beta.IsBlack())
                    {
                        terminated = true;
                        return false;
                    }

                    if (bounce < max_bounces && !ms.Le.IsBlack())
                    {
                        // Add medium emission
                        Float pdf = sigma_maj[wavelength] * T_maj[wavelength];
                        Spectrum beta_e = beta * T_maj / pdf;

                        // Rescaled sampling probability for emission event
                        Spectrum r_e = r_u * sigma_maj * T_maj / pdf;

                        if (!r_e.IsBlack())
                        {
                            // Single sample wavelength-wise MIS estimator with balance heuristic
                            L += beta_e * ms.sigma_a * ms.Le / r_e.Average();
                        }
                    }

                    Float p_absorb = ms.sigma_a[wavelength] / sigma_maj[wavelength];
                    Float p_scatter = ms.sigma_s[wavelength] / sigma_maj[wavelength];
                    Float p_null = std::max<Float>(0, 1 - p_absorb - p_scatter);
                    Float events[3] = { p_absorb, p_scatter, p_null };

                    int32 event = SampleDiscrete(events, rng.NextFloat());
                    switch (event)
                    {
                    case 0:
                    {
                        // Sampled absorption event
                        // Add medium emission with MIS weight of 0
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

                        Float pdf = T_maj[wavelength] * ms.sigma_s[wavelength];
                        beta *= T_maj * ms.sigma_s / pdf;
                        r_u *= T_maj * ms.sigma_s / pdf;

                        // Add direct light
                        Intersection medium_isect{ .point = p };
                        L += SampleDirectLight(wo, medium_isect, medium, nullptr, ms.phase, wavelength, sampler, beta, r_u);
                        last_scattering_vertex = p;

                        // Sample phase function to find next path direction
                        PhaseFunctionSample phase_sample;
                        if (!ms.phase->Sample_p(&phase_sample, wo, sampler.Next2D()))
                        {
                            terminated = true;
                        }

                        beta *= phase_sample.p / phase_sample.pdf;
                        // light sampling PDF at this vertex will be incorporated into r_l when it intersects the light source
                        r_l = r_u / phase_sample.pdf;

                        ray.o = p;
                        ray.d = phase_sample.wi;

                        specular_bounce = false;
                        any_non_specular_bounces = true;

                        scattered = true;

                        return false;
                    }

                    case 2:
                    {
                        // Sampled null scattering event, continue sampling
                        Spectrum sigma_n = Max<Float>(sigma_maj - ms.sigma_a - ms.sigma_s, 0);
                        Float pdf = T_maj[wavelength] * sigma_n[wavelength];
                        if (pdf == 0)
                        {
                            beta = Spectrum::black;
                        }
                        else
                        {
                            beta *= T_maj * sigma_n / pdf;
                        }

                        r_u *= T_maj * sigma_n / pdf;
                        r_l *= T_maj * sigma_maj / pdf; // Rescaled ratio tracking pdf

                        return !beta.IsBlack() && !r_u.IsBlack();
                    }

                    default:
                        assert(false);
                        return false;
                    }
                }
            );

            if (terminated || beta.IsBlack() || r_u.IsBlack())
            {
                break;
            }

            if (scattered)
            {
                // Continue medium sampling
                continue;
            }

            // It past the medium extent
            beta *= T_maj / T_maj[wavelength];
            r_u *= T_maj / T_maj[wavelength];
            r_l *= T_maj / T_maj[wavelength];
        }

        if (!found_intersection)
        {
            if (bounce == 0 || specular_bounce)
            {
                for (Light* light : infinite_lights)
                {
                    L += beta * light->Le(ray) / r_u.Average();
                }
            }
            else
            {
                for (Light* light : infinite_lights)
                {
                    Ray r(last_scattering_vertex, ray.d);
                    Float light_pdf = light->EvaluatePDF(ray) * light_sampler.EvaluatePMF(light);
                    r_l *= light_pdf;
                    L += beta * light->Le(ray) / (r_u + r_l).Average();
                }
            }

            break;
        }

        if (Spectrum Le = isect.Le(wo); !Le.IsBlack())
        {
            bool has_area_light = area_lights.contains(isect.primitive);
            if (bounce == 0 || specular_bounce || !has_area_light)
            {
                L += beta * Le / r_u.Average();
            }
            else if (has_area_light)
            {
                // Add emission from area light source
                AreaLight* area_light = area_lights.at(isect.primitive);

                Ray r(last_scattering_vertex, ray.d);
                Float light_pdf = area_light->EvaluatePDF(r) * light_sampler.EvaluatePMF(area_light);
                r_l *= light_pdf;
                L += beta * Le / (r_u + r_l).Average();
            }
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
            medium = isect.GetMedium(ray.d);
            ray.o = isect.point;
            --bounce;
            continue;
        }

        // Blur bsdf if possible
        if (regularize_bsdf && any_non_specular_bounces)
        {
            bsdf.Regularize();
        }

        // Estimate direct light
        if (IsNonSpecular(bsdf.Flags()))
        {
            L += SampleDirectLight(wo, isect, nullptr, &bsdf, nullptr, wavelength, sampler, beta, r_u);
        }
        last_scattering_vertex = isect.point;

        BSDFSample bsdf_sample;
        if (!bsdf.Sample_f(&bsdf_sample, wo, sampler.Next1D(), sampler.Next2D()))
        {
            break;
        }

        specular_bounce = bsdf_sample.IsSpecular();
        any_non_specular_bounces |= !bsdf_sample.IsSpecular();
        if (bsdf_sample.IsTransmission())
        {
            eta_scale *= Sqr(bsdf_sample.eta);
        }

        beta *= bsdf_sample.f * AbsDot(isect.shading.normal, bsdf_sample.wi) / bsdf_sample.pdf;
        // light sampling PDF at this vertex will be incorporated into r_l when it intersects the light source
        r_l = r_u / bsdf_sample.pdf;

        ray = Ray(isect.point, bsdf_sample.wi);
        medium = isect.GetMedium(bsdf_sample.wi);

        // Terminate path with russian roulette
        constexpr int32 min_bounces = 2;
        if (bounce > min_bounces)
        {
            Spectrum rr = beta * eta_scale / r_u.Average();
            Float p = rr.MaxComponent();
            if (p < 1 && sampler.Next1D() > p)
            {
                break;
            }

            beta /= p;
        }
    }

    return L;
}

Spectrum VolPathIntegrator::SampleDirectLight(
    const Vec3& wo,
    const Intersection& isect,
    const Medium* medium,
    const BSDF* bsdf,
    const PhaseFunction* phase,
    int32 wavelength,
    Sampler& sampler,
    Spectrum beta,
    Spectrum r_p
) const
{
    Float u0 = sampler.Next1D();
    Point2 u12 = sampler.Next2D();
    SampledLight sampled_light;
    if (!light_sampler.Sample(&sampled_light, isect, u0))
    {
        return Spectrum::black;
    }

    LightSample light_sample = sampled_light.light->Sample_Li(isect, u12);
    if (light_sample.Li.IsBlack() || light_sample.pdf == 0)
    {
        return Spectrum::black;
    }

    Float light_pdf = light_sample.pdf / sampled_light.weight;

    Spectrum scattering_f;
    Float scattering_pdf;
    Vec3 wi = light_sample.wi;

    if (bsdf)
    {
        medium = isect.GetMedium(wi);
        scattering_f = bsdf->f(wo, wi) * AbsDot(isect.shading.normal, wi);
        scattering_pdf = bsdf->PDF(wo, wi);
    }
    else
    {
        assert(medium != nullptr);
        assert(phase != nullptr);
        scattering_f = Spectrum(phase->p(wo, wi));
        scattering_pdf = phase->PDF(wo, wi);
    }

    if (scattering_f.IsBlack())
    {
        return Spectrum::black;
    }

    Ray light_ray(isect.point, wi);
    Float visibility = light_sample.visibility;

    Spectrum T_ray(1); // Transmittance estimate by ratio tracking
    Spectrum r_u(1);   // Rescaled null scattered distance sampling pdf
    Spectrum r_l(1);   // Rescaled ratio tracking pdf

    RNG rng(Hash(light_ray.o), Hash(light_ray.d));

    while (visibility > 0)
    {
        Intersection light_isect;
        bool found_intersection = Intersect(&light_isect, light_ray, Ray::epsilon, visibility);

        if (found_intersection && light_isect.primitive->GetMaterial())
        {
            // Intersects opaque surface
            return Spectrum::black;
        }

        if (medium)
        {
            Float t_max = found_intersection ? light_isect.t : visibility;
            Float u = rng.NextFloat();

            Spectrum T_maj = Sample_MajorantTransmittance(
                medium, wavelength, light_ray, t_max, u, rng,
                [&](Point3 p, MediumSample ms, Spectrum sigma_maj, Spectrum T_maj) -> bool {
                    // Estimate transmittance along the light ray by ratio tracking
                    Spectrum sigma_n = Max<Float>(sigma_maj - ms.sigma_a - ms.sigma_s, 0);
                    Float pdf = T_maj[wavelength] * sigma_maj[wavelength];
                    T_ray *= T_maj * sigma_n / pdf;
                    r_l *= T_maj * sigma_maj / pdf;
                    r_u *= T_maj * sigma_n / pdf;

                    // Stochastically terminate distance sampling with russian roulette
                    Spectrum Tr = T_ray / (r_u + r_l).Average();
                    if (Tr.MaxComponent() < 0.05f)
                    {
                        constexpr Float rr = 0.75f;
                        if (rng.NextFloat() < rr)
                        {
                            T_ray = Spectrum::black;
                        }
                        else
                        {
                            T_ray /= 1 - rr;
                        }
                    }

                    return !T_ray.IsBlack();
                }
            );

            // Update transmittance estimate for last majorant segment
            T_ray *= T_maj / T_maj[wavelength];
            r_l *= T_maj / T_maj[wavelength];
            r_u *= T_maj / T_maj[wavelength];
        }

        if (T_ray.IsBlack())
        {
            return Spectrum::black;
        }

        if (!found_intersection)
        {
            break;
        }

        // Move the ray origin toward the intersection point
        light_ray.o = light_isect.point;
        visibility -= light_isect.t;
        medium = light_isect.GetMedium(light_ray.d);
    }

    // Multiply the rescaled path probabilities for path sampling of the path
    // up to the last real-scattering vertex where direct lighting is being computed
    // Division by the light path PDF canceled out
    r_l *= r_p * light_pdf;
    r_u *= r_p * scattering_pdf;

    if (sampled_light.light->IsDeltaLight())
    {
        return beta * scattering_f * T_ray * light_sample.Li / r_l.Average();
    }
    else
    {
        return beta * scattering_f * T_ray * light_sample.Li / (r_u + r_l).Average();
    }
}

} // namespace bulbit
