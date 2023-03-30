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

    for (int32 i = 0; i < bounce_count; ++i)
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

} // namespace spt
