#include "../samples.h"

// The final scene of Ray Tracing in One Weekend
// https://raytracing.github.io/books/RayTracingInOneWeekend.html#wherenext?/afinalrender
std::unique_ptr<Camera> RaytracigInOneWeekend(Scene& scene)
{
    Medium* hm = nullptr;
    // hm = scene.CreateMedium<HomogeneousMedium>(Spectrum(0), Spectrum(0.0f), Spectrum(0.0), 0.5f);
    MediumInterface mi_outside(nullptr, hm);
    MediumInterface mi_inside(hm, nullptr);
    MediumInterface mi_two_sided(hm, hm);

    // CreateSphere(scene, identity, 100, nullptr, mi_inside);

    auto ground_material = CreateDiffuseMaterial(scene, Spectrum(0.5f, 0.5f, 0.5f));

    Transform tf = identity;
    tf.s *= 30;
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
                        scene, Transform(center, Quat(DegToRad(Rand(0, 180)), SampleUniformSphere(RandVec2()))), 0.2f, mat,
                        mi_outside
                    );
                }
                else
                {
                    // glass
                    auto glass = CreateDielectricMaterial(scene, 1.5f, Rand(0.001f, 0.3f));
                    CreateSphere(scene, center, 0.2f, glass, mi_outside);
                }
            }
        }
    }

    auto material1 = CreateDielectricMaterial(scene, 1.5f);
    CreateSphere(scene, Vec3(0, 1, 0), 1.0f, material1, mi_outside);

    auto material2 = CreateDiffuseMaterial(scene, Spectrum(0.4f, 0.2f, 0.1f));
    CreateSphere(scene, Vec3(-4, 1, 0), 1.0f, material2, mi_outside);

    auto material3 = CreateConductorMaterial(scene, { 0.1, 0.2, 1.9 }, { 3, 2.5, 2 }, 0.01f);
    CreateSphere(scene, Transform(Vec3(4, 1, 0), Quat(DegToRad(0), Normalize(Vec3(1, 0, 0)))), 1.0f, material3, mi_outside);

    // CreateImageInfiniteLight(scene, "res/HDR/kloppenheim_07_puresky_1k.hdr");
    // CreateImageInfiniteLight(scene, "res/HDR/quarry_04_puresky_1k.hdr");
    // CreateImageInfiniteLight(scene, "res/HDR/photo_studio_01_1k.hdr");
    // CreateImageInfiniteLight(scene, "res/HDR/pizzo_pernice_1k.hdr");
    CreateImageInfiniteLight(scene, "res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(pi, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/harties_4k.hdr");
    // CreateImageInfiniteLight(scene, "res/sunflowers/sunflowers_puresky_4k.hdr");
    // CreateImageInfiniteLight(scene, "res/solitude_night_4k/solitude_night_4k.hdr");
    // CreateImageInfiniteLight(scene, "res/earthmap.jpg");

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

    return std::make_unique<PerspectiveCamera>(
        lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus, Point2i(width, height), hm
    );
    // return std::make_unique<SphericalCamera>(lookfrom, Point2i(width, height));
}

static int32 index = Sample::Register("rtow", RaytracigInOneWeekend);
