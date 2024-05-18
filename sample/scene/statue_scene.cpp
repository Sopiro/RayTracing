#include "../samples.h"
#include "bulbit/camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

std::unique_ptr<Camera> StatueScene(Scene& scene)
{
    {
        auto mat = scene.CreateMaterial<UnrealMaterial>(ConstantColorTexture::Create(1.0), ConstantFloatTexture::Create(1.0f),
                                                        ConstantFloatTexture::Create(0.1f));
        // auto mat = scene.CreateMaterial<Dielectric>(1.5f);

        auto tf = Transform{ Point3(0.0f, -2.0f, 0.0f), Quat(DegToRad(45.0f), y_axis), Vec3(20.0f) };
        SetLoaderFallbackMaterial(mat);
        LoadModel(scene, "res/horse_statue_01_4k/horse_statue_01_4k.gltf", tf);

        // auto tf = Transform{ Point3(0.0f, -2.0f, 0.0f), Quat(DegToRad(0.0f), y_axis), Vec3(8.0f) };
        // LoadModel(scene, "res/marble_bust_01_4k/marble_bust_01_4k.gltf", tf);
    }

    Float light = 8.0f;
    Float distance = 5.0f;
    Float size = 2.0f;

    {
        auto red = scene.CreateMaterial<DiffuseLight>(ConstantColorTexture::Create(Spectrum(light, 0.0f, 0.0f)));
        auto tf = Transform{ Point3(-distance, 0.0f, 0.0f), identity, Vec3(1.0f, size, size) };
        CreateRectYZ(scene, tf, red, true);
    }

    {
        auto blue = scene.CreateMaterial<DiffuseLight>(ConstantColorTexture::Create(Spectrum(0.0f, 0.0f, light)));
        auto tf = Transform{ Point3(distance, 0.0f, 0.0f), Quat(pi, y_axis), Vec3(1.0f, size, size) };
        CreateRectYZ(scene, tf, blue, true);
    }

    {
        auto white = scene.CreateMaterial<DiffuseLight>(ConstantColorTexture::Create(Spectrum(0.5f)));

        auto tf = Transform{ Point3(0.0f, 4.0f, 0.0f), Quat(pi, x_axis), Vec3(8.0f, 1.0f, 8.0f) };
        CreateRectXZ(scene, tf, white, true);
    }

    // {
    // auto white = scene.CreateMaterial<DiffuseLight>(SolidColor::Create(Spectrum(3.0f)));

    //     int32 count = 10;
    //     Float d = two_pi / count;

    //     Float y = 2.0f;
    //     Float r = 1.5f;

    //     for (int i = 0; i < count; ++i)
    //     {
    //         Float angle = d * i;
    //         auto pos = Vec3(std::cos(angle) * r, y, std::sin(angle) * r);
    //         auto sphere = std::make_shared<Sphere>(pos, 0.1f, white);

    //         scene.AddLight(sphere);
    //     }
    // }

    // Floor
    {
        auto mat = scene.CreateMaterial<UnrealMaterial>(ConstantColorTexture::Create(1.0f), ConstantFloatTexture::Create(0.0f),
                                                        ConstantFloatTexture::Create(0.01f));
        auto tf = Transform{ Point3(0.0f, -2.0f, 0.0f), identity, Vec3(8.0f, 1.0f, 8.0f) };
        CreateRectXZ(scene, tf, mat);

        // mat = RandomPBRMaterial();
        // mat->basecolor_map = SolidColor::Create(Vec3(1.0f));
        // mat->metallic_map = SolidColor::Create(Vec3(1.0f));
        // mat->roughness_map = SolidColor::Create(Vec3(0.05));

        tf = Transform{ Point3(0.0f, 0.0f, -4.0f), identity, Vec3(8.0f, 8.0f, 1.0f) };
        CreateRectXY(scene, tf, mat);
    }

    // scene.CreateLight<ImageInfiniteLight>("res/solitude_night_4k/solitude_night_4k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/sunflowers/sunflowers_puresky_4k.hdr");

    // Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    Float aspect_ratio = 1.f;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0, 0, 10 };
    Point3 lookat{ 0, 0, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 30;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("statue", StatueScene);
