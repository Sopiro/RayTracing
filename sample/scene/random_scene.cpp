#include "spt/spt.h"

namespace spt
{

// The final scene of Ray Tracing in One Weekend
// https://raytracing.github.io/books/RayTracingInOneWeekend.html#wherenext?/afinalrender
void RandomScene(Scene& scene)
{
    auto ground_material = CreateSharedRef<Lambertian>(Spectrum(0.5, 0.5, 0.5));
    scene.Add(CreateSharedRef<Sphere>(Vec3(0, -1000, 0), 1000, ground_material));

    for (int32 a = -11; a < 11; a++)
    {
        for (int32 b = -11; b < 11; b++)
        {
            auto choose_mat = Rand();
            Vec3 center(a + 0.9 * Rand(), 0.2, b + 0.9 * Rand());

            if ((center - Vec3(4, 0.2, 0)).Length() > 0.9)
            {
                Ref<Material> sphere_material;

                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto albedo = RandVec3() * RandVec3();
                    sphere_material = CreateSharedRef<Lambertian>(albedo);
                    scene.Add(CreateSharedRef<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = RandVec3(0.5, 1.0);
                    auto fuzz = Rand(0.0, 0.5);
                    sphere_material = CreateSharedRef<Metal>(albedo, fuzz);
                    scene.Add(CreateSharedRef<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = CreateSharedRef<Dielectric>(1.5);
                    scene.Add(CreateSharedRef<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = CreateSharedRef<Dielectric>(1.5);
    scene.Add(CreateSharedRef<Sphere>(Vec3(0, 1, 0), 1.0, material1));

    auto material2 = CreateSharedRef<Lambertian>(Spectrum(0.4, 0.2, 0.1));
    scene.Add(CreateSharedRef<Sphere>(Vec3(-4, 1, 0), 1.0, material2));

    auto material3 = CreateSharedRef<Metal>(Spectrum(0.7, 0.6, 0.5), 0.0);
    scene.Add(CreateSharedRef<Sphere>(Vec3(4, 1, 0), 1.0, material3));

    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/kloppenheim_07_puresky_1k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/scythian_tombs_2_4k.hdr.hdr"));
    scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/quarry_04_puresky_1k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/photo_studio_01_1k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/pizzo_pernice_1k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/harties_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/solitude_night_4k/solitude_night_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/earthmap.jpg"));
}

} // namespace spt
