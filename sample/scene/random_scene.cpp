#include "spt/spt.h"

namespace spt
{

// The final scene of Ray Tracing in One Weekend
// https://raytracing.github.io/books/RayTracingInOneWeekend.html#wherenext?/afinalrender
void RandomScene(Scene& scene)
{
    auto ground_material = CreateSharedRef<Lambertian>(Spectrum(0.5, 0.5, 0.5));
    scene.Add(CreateSharedRef<Sphere>(Vec3(0, -1000, 0), 1000, ground_material));

    Srand(1);

    for (int32 a = -11; a < 11; a++)
    {
        for (int32 b = -11; b < 11; b++)
        {
            auto choose_mat = Rand();
            Vec3 center(a + 0.9 * Rand(), 0.2, b + 0.9 * Rand());

            if ((center - Vec3(4, 0.2, 0)).Length() > 0.9)
            {
                if (choose_mat < 0.9)
                {
                    auto mat = RandomMicrofacetMaterial();
                    mat->roughness = ConstantColor::Create(Rand(0, 1));
                    mat->emissive = ConstantColor::Create(0);
                    scene.Add(CreateSharedRef<Sphere>(center, 0.2, mat));
                }
                else
                {
                    // glass
                    auto glass = CreateSharedRef<Dielectric>(1.5);
                    scene.Add(CreateSharedRef<Sphere>(center, 0.2, glass));
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
    scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/quarry_04_puresky_1k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/photo_studio_01_1k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/pizzo_pernice_1k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(pi, y_axis))));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/harties_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/solitude_night_4k/solitude_night_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/earthmap.jpg"));
}

} // namespace spt
