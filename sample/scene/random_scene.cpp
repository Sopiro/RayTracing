#include "../samples.h"
#include "bulbit/dielectric.h"
#include "bulbit/diffuse_light.h"
#include "bulbit/lambertian.h"
#include "bulbit/metal.h"
#include "bulbit/perspective_camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

// The final scene of Ray Tracing in One Weekend
// https://raytracing.github.io/books/RayTracingInOneWeekend.html#wherenext?/afinalrender
Camera* RaytracigInOneWeekend(Scene& scene)
{
    auto ground_material = CreateSharedRef<Lambertian>(Spectrum(0.5f, 0.5f, 0.5f));
    scene.Add(CreateSharedRef<Sphere>(Vec3(0, -1000, 0), 1000.0f, ground_material));

    Srand(7777);

    for (int32 a = -11; a < 11; a++)
    {
        for (int32 b = -11; b < 11; b++)
        {
            auto choose_mat = Rand();
            Vec3 center(a + 0.9f * Rand(), 0.2f, b + 0.9f * Rand());

            if ((center - Vec3(4, 0.2f, 0)).Length() > 0.9f)
            {
                if (choose_mat < 0.9f)
                {
                    RandomMicrofacetMaterial();
                    auto mat = CreateSharedRef<Microfacet>(
                        ConstantColor::Create(Spectrum(Rand(0.0f, 1.0f), Rand(0.0f, 1.0f), Rand(0.0f, 1.0f)) * Float(0.7f)),
                        ConstantColor::Create(Spectrum(Rand() > 0.5f ? Float(1.0f) : Float(0.0f))),
                        ConstantColor::Create(Rand(0, 1)));
                    scene.Add(CreateSharedRef<Sphere>(center, 0.2f, mat));
                }
                else
                {
                    // glass
                    auto glass = CreateSharedRef<Dielectric>(1.5f);
                    scene.Add(CreateSharedRef<Sphere>(center, 0.2f, glass));
                }
            }
        }
    }

    auto material1 = CreateSharedRef<Dielectric>(1.5f);
    scene.Add(CreateSharedRef<Sphere>(Vec3(0, 1, 0), 1.0f, material1));

    auto material2 = CreateSharedRef<Lambertian>(Spectrum(0.4f, 0.2f, 0.1f));
    scene.Add(CreateSharedRef<Sphere>(Vec3(-4, 1, 0), 1.0f, material2));

    auto material3 = CreateSharedRef<Metal>(Spectrum(0.7f, 0.6f, 0.5f), 0.0f);
    scene.Add(CreateSharedRef<Sphere>(Vec3(4, 1, 0), 1.0f, material3));

    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/kloppenheim_07_puresky_1k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/quarry_04_puresky_1k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/photo_studio_01_1k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/pizzo_pernice_1k.hdr"));
    scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(pi, y_axis))));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/harties_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/solitude_night_4k/solitude_night_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/earthmap.jpg"));

    Float aspect_ratio = 16.0f / 9.0f;

    Point3 lookfrom{ 13, 2, 3 };
    Point3 lookat{ 0, 0, 0 };

    Float dist_to_focus = 10.0f;
    Float aperture = 0.1f;
    Float vFov = 20;

    return new PerspectiveCamera(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
}

static int32 index = Sample::Register("rtow", RaytracigInOneWeekend);

} // namespace bulbit
