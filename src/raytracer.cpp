#pragma once

#include "raytracer/raytracer.h"

namespace spt
{

Color ComputeRayColor(
    const Ray& ray, const Hittable& scene, std::shared_ptr<Hittable>& lights, const Color& sky_color, int32 depth)
{
    if (depth <= 0)
    {
        return Color{ 0.0 };
    }

    HitRecord rec;
    if (scene.Hit(ray, ray_tolerance, infinity, rec) == false)
    {
        return sky_color;
    }

    Color emitted = rec.mat->Emit(ray, rec);

    ScatterRecord srec;
    if (rec.mat->Scatter(ray, rec, srec) == false)
    {
        return emitted;
    }

    if (srec.is_specular == true)
    {
        return srec.attenuation * ComputeRayColor(srec.specular_ray, scene, lights, sky_color, depth - 1);
    }

#if IMPORTANCE_SAMPLING
    HittablePDF light_pdf{ lights, rec.point };
    MixturePDF mixed_pdf{ &light_pdf, srec.pdf.get() };

    Ray scattered{ rec.point, mixed_pdf.Generate() };

    // Scattering rays must be directed outward from the surface
    if (Dot(scattered.dir, rec.normal) < 0.0)
    {
        scattered.dir.Negate();
    }

    double pdf_value = mixed_pdf.Evaluate(scattered.dir);

    return emitted + srec.attenuation * rec.mat->ScatteringPDF(ray, rec, scattered) *
                         ComputeRayColor(scattered, scene, lights, sky_color, depth - 1) / pdf_value;
#else
    Ray scattered{ rec.point + rec.normal * ray_tolerance, srec.pdf->Generate() };
    double pdf_value = srec.pdf->Evaluate(scattered.dir);

    return emitted + srec.attenuation * rec.mat->ScatteringPDF(ray, rec, scattered) *
                         ComputeRayColor(scattered, scene, lights, sky_color, depth - 1) / pdf_value;
#endif
}

Color PathTrace(Ray ray, const Hittable& scene, std::shared_ptr<Hittable>& lights, const Color& sky_color, int32 bounce_count)
{
    Color accu{ 0.0, 0.0, 0.0 };
    Color abso{ 1.0, 1.0, 1.0 };

    for (int32 i = 0; i < bounce_count; ++i)
    {
        HitRecord rec;
        if (scene.Hit(ray, ray_tolerance, infinity, rec) == false)
        {
            accu += sky_color * abso;
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

#if IMPORTANCE_SAMPLING
        HittablePDF light_pdf{ lights, rec.point };
        MixturePDF mixed_pdf{ &light_pdf, srec.pdf.get() };

        Ray scattered{ rec.point, mixed_pdf.Generate() };
        double pdf_value = mixed_pdf.Evaluate(scattered.dir);

        // Scattering rays must be directed outward from the surface
        if (Dot(scattered.dir, rec.normal) < 0.0)
        {
            scattered.dir.Negate();
        }

        accu += emitted * abso;
        abso *= srec.attenuation * rec.mat->ScatteringPDF(ray, rec, scattered) / pdf_value;
        ray = scattered;

#else
        Ray scattered{ rec.point, srec.pdf->Generate() };
        double pdf_value = srec.pdf->Evaluate(scattered.dir);

        accu += emitted * abso;
        abso *= srec.attenuation * rec.mat->ScatteringPDF(ray, rec, scattered) / pdf_value;
        ray = scattered;
#endif
    }

    return accu;
}

} // namespace spt
