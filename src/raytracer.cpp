#pragma once

#include "raytracer/raytracer.h"

namespace spt
{

Color ComputeRayColor(const Scene& scene, const Ray& ray, int32 bounce_count)
{
    if (bounce_count <= 0)
    {
        return Color{ 0.0, 0.0, 0.0 };
    }

    HitRecord rec;
    if (scene.Hit(ray, ray_tolerance, infinity, rec) == false)
    {
        return scene.GetSkyColor(ray.dir);
    }

    // Outgoing radiance
    Color rad = rec.mat->Emit(ray, rec);

    ScatterRecord srec;
    if (rec.mat->Scatter(ray, rec, srec) == false)
    {
        return rad;
    }

    if (srec.is_specular == true)
    {
        rad += srec.attenuation * ComputeRayColor(scene, srec.specular_ray, bounce_count - 1);
        return rad;
    }

    // Direct illumination by directional light
    if (scene.HasDirectionalLight())
    {
        auto sun = scene.GetDirectionalLight();
        Ray to_sun{ rec.point + rec.normal * ray_tolerance, -sun->dir };
        HitRecord rec2;
        if (scene.Hit(to_sun, ray_tolerance, infinity, rec2) == false)
        {
            rad += rec.mat->Evaluate(ray, rec, to_sun) * sun->radiance;
        }
    }

    if (scene.HasLights() && importance_sampling)
    {
        HittablePDF light_pdf{ &scene.GetLights(), rec.point };
        MixturePDF mixed_pdf{ &light_pdf, srec.pdf.get() };

        Ray scattered{ rec.point, mixed_pdf.Generate() };

        // Scattering rays must be directed outward from the surface
        if (Dot(scattered.dir, rec.normal) < 0.0)
        {
            scattered.dir.Negate();
        }

        double pdf_value = mixed_pdf.Evaluate(scattered.dir);

        rad += rec.mat->Evaluate(ray, rec, scattered) * ComputeRayColor(scene, scattered, bounce_count - 1) / pdf_value;
        return rad;
    }
    else
    {
        Ray scattered{ rec.point, srec.pdf->Generate() };
        double pdf_value = srec.pdf->Evaluate(scattered.dir);

        rad += rec.mat->Evaluate(ray, rec, scattered) * ComputeRayColor(scene, scattered, bounce_count - 1) / pdf_value;
        return rad;
    }
}

Color PathTrace(const Scene& scene, Ray ray, int32 bounce_count)
{
    Color accu{ 0.0, 0.0, 0.0 };
    Color abso{ 1.0, 1.0, 1.0 };

    for (int32 bounce = 0; bounce < bounce_count; ++bounce)
    {
        HitRecord rec;
        if (scene.Hit(ray, ray_tolerance, infinity, rec) == false)
        {
            accu += scene.GetSkyColor(ray.dir) * abso;
            break;
        }

        Color emitted = rec.mat->Emit(ray, rec);

        ScatterRecord srec;
        if (rec.mat->Scatter(ray, rec, srec) == false)
        {
            accu += emitted * abso;
            break;
        }

        if (srec.is_specular == true)
        {
            abso *= srec.attenuation;
            ray = srec.specular_ray;
            continue;
        }

        // Direct illumination by directional light
        if (scene.HasDirectionalLight())
        {
            auto sun = scene.GetDirectionalLight();
            Ray to_sun{ rec.point + rec.normal * ray_tolerance, -sun->dir };
            HitRecord rec2;
            if (scene.Hit(to_sun, ray_tolerance, infinity, rec2) == false)
            {
                accu += rec.mat->Evaluate(ray, rec, to_sun) * sun->radiance * abso;
            }
        }

        if (scene.HasLights() && importance_sampling)
        {
            HittablePDF light_pdf{ &scene.GetLights(), rec.point };
            MixturePDF mixed_pdf{ &light_pdf, srec.pdf.get() };

            Ray scattered{ rec.point, mixed_pdf.Generate() };

            // Scattering rays must be directed outward from the surface
            if (Dot(scattered.dir, rec.normal) < 0.0)
            {
                scattered.dir.Negate();
            }

            double pdf_value = mixed_pdf.Evaluate(scattered.dir);

            accu += emitted * abso;
            abso *= rec.mat->Evaluate(ray, rec, scattered) / pdf_value;
            ray = scattered;
        }
        else
        {
            Ray scattered{ rec.point, srec.pdf->Generate() };
            double pdf_value = srec.pdf->Evaluate(scattered.dir);

            accu += emitted * abso;
            abso *= rec.mat->Evaluate(ray, rec, scattered) / pdf_value;
            ray = scattered;
        }
    }

    return accu;
}

Color PathTrace2(const Scene& scene, Ray ray, int32 bounce_count)
{
    Color accu{ 0.0 };
    Color abso{ 1.0 };
    bool was_specular = false;

    for (int32 bounce = 0; bounce < bounce_count; ++bounce)
    {
        HitRecord rec;
        if (scene.Hit(ray, ray_tolerance, infinity, rec) == false)
        {
            accu += scene.GetSkyColor(ray.dir) * abso;
            break;
        }

        Color emitted = rec.mat->Emit(ray, rec);

        ScatterRecord srec;
        if (rec.mat->Scatter(ray, rec, srec) == false)
        {
            if (bounce == 0 || was_specular == true)
            {
                accu += emitted * abso;
            }
            break;
        }

        if (srec.is_specular == true)
        {
            abso *= srec.attenuation;
            ray = srec.specular_ray;
            was_specular = true;
            continue;
        }
        else
        {
            was_specular = false;
        }

        // Direct illuminations

        if (scene.HasDirectionalLight())
        {
            auto sun = scene.GetDirectionalLight();
            Ray to_sun{ rec.point + rec.normal * ray_tolerance, -sun->dir };
            HitRecord rec2;
            if (scene.Hit(to_sun, ray_tolerance, infinity, rec2) == false)
            {
                accu += rec.mat->Evaluate(ray, rec, to_sun) * sun->radiance * abso;
            }
        }

        if (scene.HasLights())
        {
            auto& lights = scene.GetLights().GetObjects();

            for (int32 i = 0; i < lights.size(); ++i)
            {
                HittablePDF light_pdf{ lights[i].get(), rec.point };

                // Importance sample light
                Ray to_light{ rec.point, light_pdf.Generate() };
                double light_brdf_p = srec.pdf->Evaluate(to_light.dir);

                if (light_brdf_p > 0.0 && Dot(to_light.dir, rec.normal) > 0.0)
                {
                    double light_p = light_pdf.Evaluate(to_light.dir);
                    double light_w = PowerHeuristic(light_p, light_brdf_p);

                    HitRecord rec2;
                    if (scene.Hit(to_light, ray_tolerance, infinity, rec2))
                    {
                        accu += rec2.mat->Emit(to_light, rec2) * rec.mat->Evaluate(ray, rec, to_light) * abso * light_w / light_p;
                    }
                }

                // Importance sample BRDF
                Ray scattered{ rec.point, srec.pdf->Generate() };
                double brdf_light_p = light_pdf.Evaluate(scattered.dir);

                if (brdf_light_p > 0.0)
                {
                    double brdf_p = srec.pdf->Evaluate(scattered.dir);
                    double brdf_w = PowerHeuristic(brdf_p, brdf_light_p);

                    HitRecord rec2;
                    if (scene.Hit(scattered, ray_tolerance, infinity, rec2))
                    {
                        accu += rec2.mat->Emit(scattered, rec2) * rec.mat->Evaluate(ray, rec, scattered) * abso * brdf_w / brdf_p;
                    }
                }
            }
        }

        // Sample new search direction based on BRDF
        Ray scattered{ rec.point, srec.pdf->Generate() };
        double pdf_value = srec.pdf->Evaluate(scattered.dir);

        accu += emitted * abso;
        abso *= rec.mat->Evaluate(ray, rec, scattered) / pdf_value;
        ray = scattered;

        // Russian roulette
        if (bounce > 2)
        {
            double rr = fmax(abso.x, fmax(abso.y, abso.z));
            if (Rand() > rr)
            {
                break;
            }

            abso *= 1.0 / rr;
        }
    }

    return accu;
}

} // namespace spt
