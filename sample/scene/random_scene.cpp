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
std::unique_ptr<Camera> RaytracigInOneWeekend(Scene& scene)
{
    auto ground_material = std::make_shared<Lambertian>(Spectrum(0.5f, 0.5f, 0.5f));
    scene.Add(std::make_shared<Sphere>(Vec3(0, -1000, 0), 1000.0f, ground_material));

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
                    auto mat = std::make_shared<Microfacet>(
                        ConstantColor::Create(Spectrum(Rand(0.0f, 1.0f), Rand(0.0f, 1.0f), Rand(0.0f, 1.0f)) * Float(0.7f)),
                        ConstantColor::Create(Spectrum(Rand() > 0.5f ? Float(1.0f) : Float(0.0f))),
                        ConstantColor::Create(Rand(0, 1)));
                    scene.Add(std::make_shared<Sphere>(center, 0.2f, mat));
                }
                else
                {
                    // glass
                    auto glass = std::make_shared<Dielectric>(1.5f);
                    scene.Add(std::make_shared<Sphere>(center, 0.2f, glass));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5f);
    scene.Add(std::make_shared<Sphere>(Vec3(0, 1, 0), 1.0f, material1));

    auto material2 = std::make_shared<Lambertian>(Spectrum(0.4f, 0.2f, 0.1f));
    scene.Add(std::make_shared<Sphere>(Vec3(-4, 1, 0), 1.0f, material2));

    auto material3 = std::make_shared<Metal>(Spectrum(0.7f, 0.6f, 0.5f), 0.0f);
    scene.Add(std::make_shared<Sphere>(Vec3(4, 1, 0), 1.0f, material3));

    // scene.AddLight(std::make_shared<InfiniteAreaLight>("res/HDR/kloppenheim_07_puresky_1k.hdr"));
    // scene.AddLight(std::make_shared<InfiniteAreaLight>("res/HDR/quarry_04_puresky_1k.hdr"));
    // scene.AddLight(std::make_shared<InfiniteAreaLight>("res/HDR/photo_studio_01_1k.hdr"));
    // scene.AddLight(std::make_shared<InfiniteAreaLight>("res/HDR/pizzo_pernice_1k.hdr"));
    scene.AddLight(std::make_shared<InfiniteAreaLight>("res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(pi, y_axis))));
    // scene.AddLight(std::make_shared<InfiniteAreaLight>("res/HDR/harties_4k.hdr"));
    // scene.AddLight(std::make_shared<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr"));
    // scene.AddLight(std::make_shared<InfiniteAreaLight>("res/solitude_night_4k/solitude_night_4k.hdr"));
    // scene.AddLight(std::make_shared<InfiniteAreaLight>("res/earthmap.jpg"));

    Float aspect_ratio = 16. / 9.;
    // Float aspect_ratio = 3. / 2.;
    // Float aspect_ratio = 4. / 3.;
    // Float aspect_ratio = 1.;
    int32 width = 1920;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 13, 2, 3 };
    Point3 lookat{ 0, 0, 0 };

    Float dist_to_focus = 10.0f;
    Float aperture = 0.1f;
    Float vFov = 20;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("rtow", RaytracigInOneWeekend);

} // namespace bulbit
