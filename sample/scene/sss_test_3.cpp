#include "../samples.h"

std::unique_ptr<Camera> SSSTest3(Scene& scene)
{
    SetLoaderUseForceFallbackMaterial(true);
    // Ajax

    Spectrum sigma_a = Spectrum(1.2, 2.0, 3.5);
    Spectrum sigma_s = Spectrum(80, 100, 120) * 5;

    HomogeneousMedium* hm = scene.CreateMedium<HomogeneousMedium>(sigma_a, sigma_s, Spectrum(0.0), 0.9f);
    MediumInterface mi(hm, nullptr);

    // auto mat = scene.CreateMaterial<DielectricMaterial>(1.33f, 0.1f);
    auto mat = scene.CreateMaterial<SubsurfaceMaterial>(Spectrum(1.0), Spectrum(1) / (sigma_a + sigma_s), 1.0f, 0.0f);
    SetLoaderFallbackMaterial(nullptr);
    SetLoaderFallbackMediumInterface(mi);

    auto tf = Transform{ Vec3(0.0f, -0.2f, 0.0f), Quat(DegToRad(180.0f), y_axis), Vec3(0.4f) };
    LoadModel(scene, "res/ajax/ajax.obj", tf);

    // scene.CreateLight<ImageInfiniteLight>("res/HDR/material-test.hdr", Transform(Quat(pi / 2, y_axis)));
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/sunset.hdr", Transform(Quat(pi / 2, y_axis)));
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/aerodynamics_workshop_1k.hdr", Transform(Quat(pi, y_axis)));
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/scythian_tombs_2_4k.hdr", Transform(Quat(0, y_axis)));
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/quarry_04_puresky_1k.hdr", Transform(Quat(0, y_axis)));
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/photo_studio_01_1k.hdr", Transform(Quat(0, y_axis)));
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/peppermint_powerplant_4k.hdr", Transform(Quat(0, y_axis)));
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/white_cliff_top_1k.hdr", Transform(Quat(pi, y_axis)));
    // scene.CreateLight<ImageInfiniteLight>("res/sunflowers/sunflowers_puresky_4k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(pi / 2, y_axis)));
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/Background_05.hdr", Transform(Quat(pi / 2, y_axis)));
    // scene.CreateLight<UniformInfiniteLight>(Spectrum(1));

    auto light = scene.CreateMaterial<DiffuseLightMaterial>(Spectrum(3.0f));
    tf = Transform{ 0, 1.0f, 0, Quat(pi, x_axis), Vec3(1.0f) };
    CreateRectXZ(scene, tf, light);

    // Floor
    {
        auto a = ConstantColorTexture::Create(0.75, 0.75, 0.75);
        auto b = ConstantColorTexture::Create(0.3, 0.3, 0.3);
        auto checker = CheckerTexture::Create(a, b, Point2(20));
        auto tf = Transform{ Vec3(0, -0.2f, 0), Quat::FromEuler({ 0, 0, 0 }), Vec3(3) };
        auto floor = scene.CreateMaterial<DiffuseMaterial>(checker);
        SetLoaderFallbackMaterial(floor);
        LoadModel(scene, "res/background.obj", tf);
    }

    // Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 9.f / 16.f;
    // Float aspect_ratio = 3.f / 2.f;
    Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 1000;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0, 0, 1 };
    Point3 lookat{ 0, 0, 0 };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0.02f;
    Float vFov = 30;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus, Point2i(width, height));
}

static int32 index = Sample::Register("sss3", SSSTest3);
