#include "../samples.h"
#include "bulbit/camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"

// The final scene of Ray Tracing in One Weekend
// https://raytracing.github.io/books/RayTracingInOneWeekend.html#wherenext?/afinalrender
std::unique_ptr<Camera> RaytracigInOneWeekend(Scene& scene)
{
    auto ground_material = scene.CreateMaterial<DiffuseMaterial>(Spectrum(0.5f, 0.5f, 0.5f));

    Transform tf = identity;
    tf.r *= 30;
    CreateRectXZ(scene, tf, ground_material);

    Srand(7777);

    for (int32 a = -11; a < 11; a++)
    {
        for (int32 b = -11; b < 11; b++)
        {
            auto choose_mat = Rand();
            Vec3 center(a + 0.9f * Rand(), 0.2f, b + 0.9f * Rand());

            if (Dist(center, Vec3(4, 0.2f, 0)) > 0.9f)
            {
                if (choose_mat < 0.9f)
                {
                    auto mat = CreateRandomUnrealMaterial(scene);
                    CreateSphere(
                        scene, Transform(center, Quat(DegToRad(Rand(0, 180)), SampleUniformSphere(RandVec2()))), 0.2f, mat
                    );
                }
                else
                {
                    // glass
                    auto glass = scene.CreateMaterial<DielectricMaterial>(1.5f, ConstantFloatTexture::Create(Rand(0.001f, 0.3f)));
                    CreateSphere(scene, center, 0.2f, glass);
                }
            }
        }
    }

    auto material1 = scene.CreateMaterial<DielectricMaterial>(1.5f);
    CreateSphere(scene, Vec3(0, 1, 0), 1.0f, material1);

    auto material2 = scene.CreateMaterial<DiffuseMaterial>(Spectrum(0.4f, 0.2f, 0.1f));
    CreateSphere(scene, Vec3(-4, 1, 0), 1.0f, material2);

    auto material3 = scene.CreateMaterial<ConductorMaterial>(
        ConstantColorTexture::Create(0.1, 0.2, 1.9), ConstantColorTexture::Create(3, 2.5, 2), ConstantFloatTexture::Create(0.01f),
        ConstantFloatTexture::Create(0.01f)
    );
    CreateSphere(scene, Transform(Vec3(4, 1, 0), Quat(DegToRad(0), Normalize(Vec3(1, 0, 0)))), 1.0f, material3);

    // scene.CreateLight<ImageInfiniteLight>("res/HDR/kloppenheim_07_puresky_1k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/quarry_04_puresky_1k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/photo_studio_01_1k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/pizzo_pernice_1k.hdr");
    scene.CreateLight<ImageInfiniteLight>("res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(pi, y_axis)));
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/harties_4k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/sunflowers/sunflowers_puresky_4k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/solitude_night_4k/solitude_night_4k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/earthmap.jpg");

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 960;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 13, 2, 3 };
    Point3 lookat{ 0, 0, 0 };

    Float dist_to_focus = 10.0f;
    Float aperture = 0.1f;
    Float vFov = 20;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
    // return std::make_unique<SphericalCamera>(lookfrom, width, height);
}

static int32 index = Sample::Register("rtow", RaytracigInOneWeekend);
