#include "../samples.h"

std::unique_ptr<Camera> MaterialTest(Scene& scene)
{
    HomogeneousMedium* hm = scene.CreateMedium<HomogeneousMedium>(Spectrum(0, 0, 0), Spectrum(10), Spectrum(0.0), -0.9f);
    MediumInterface mi(hm, nullptr);

    SetLoaderUseForceFallbackMaterial(true);

    // Floor
    {
        auto a = ConstantColorTexture::Create(0.75, 0.75, 0.75);
        auto b = ConstantColorTexture::Create(0.3, 0.3, 0.3);
        auto checker = CheckerTexture::Create(a, b, Point2(20));
        auto tf = Transform{ Vec3(0, 0, 0), Quat::FromEuler({ 0, 0, 0 }), Vec3(3) };
        auto floor = scene.CreateMaterial<DiffuseMaterial>(checker);
        SetLoaderFallbackMaterial(floor);
        LoadModel(scene, "res/background.obj", tf);
    }

    Float scale = 2.0f;

    int32 w = 3;
    int32 h = 1;

    Vec3 o(0, 0, 0);
    Vec3 x(0.9, 0, 0);
    Vec3 y(0, 0, 1);

    for (int32 j = 0; j < h; ++j)
    {
        for (int32 i = 0; i < w; ++i)
        {
            int32 sign_i = std::pow<int32>(-1, i);
            int32 sign_j = std::pow<int32>(-1, j);
            Vec3 p = o + (sign_j * y * ((j + 1) / 2)) + (sign_i * x * ((i + 1) / 2));

            // auto normalmap = ColorImageTexture::Create("res/bistro/Concrete_Normal.png");
            auto normalmap = nullptr;
            // auto mat = scene.CreateMaterial<DiffuseMaterial>(Spectrum(0.9), normalmap);
            // auto mat = scene.CreateMaterial<ConductorMaterial>(
            //     ConstantColorTexture::Create(0.1, 0.2, 1.9), ConstantColorTexture::Create(3, 2.5, 2),
            //     ConstantFloatTexture::Create(0.05f), ConstantFloatTexture::Create(0.4f), normalmap
            // );
            // auto mat = scene.CreateMaterial<DielectricMaterial>(1.5f, ConstantFloatTexture::Create(0.0f));
            // auto mat = scene.CreateMaterial<ThinDielectricMaterial>(1.5f);

            // Srand(1213212);
            // auto mat = CreateRandomUnrealMaterial(scene);

            // SetLoaderFallbackMediumInterface(mi);

            auto tf = Transform{ p, Quat::FromEuler({ 0, 0, 0 }), Vec3(scale) };

            // https://github.com/lighttransport/lighttransportequation-orb
            auto mat = scene.CreateMaterial<MirrorMaterial>(Spectrum(0.7f), normalmap);
            SetLoaderFallbackMaterial(mat);
            LoadModel(scene, "res/mori_knob/base.obj", tf);
            LoadModel(scene, "res/mori_knob/outer.obj", tf);
            SetLoaderFallbackMaterial(scene.CreateMaterial<DielectricMaterial>(1.5f, ConstantFloatTexture::Create(0.0f)));

            LoadModel(scene, "res/mori_knob/inner.obj", tf);
            LoadModel(scene, "res/mori_knob/equation.obj", tf);
        }
    }

    // scene.CreateLight<ImageInfiniteLight>(
    //     "res/material_test_ball/envmap.hdr", Transform(Quat::FromEuler(0, DegToRad(-67.26139831542969), 0))
    // );
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/photo_studio_loft_hall_1k.hdr", Transform(Quat(pi, y_axis)));
    scene.CreateLight<ImageInfiniteLight>("res/HDR/aerodynamics_workshop_1k.hdr", Transform(Quat(pi, y_axis)));
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/photo_studio_01_1k.hdr", Transform(Quat(0, y_axis)));
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/scythian_tombs_2_4k.hdr", Transform(Quat(pi, y_axis)));
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/material-test.hdr", Transform(Quat(0, y_axis)));
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/peppermint_powerplant_4k.hdr", Transform(Quat(0, y_axis)));
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/quarry_04_puresky_1k.hdr", Transform(Quat(0, y_axis)));
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/solitude_night_1k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/sunflowers/sunflowers_puresky_4k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(-pi, y_axis)));
    // scene.CreateLight<UniformInfiniteLight>(Spectrum(1));

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 9.f / 16.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 1600;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0, 1.0, 3 };
    Point3 lookat{ 0.0, 0.3, 0.0 };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0.0f;
    Float vFov = 30.0;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus, Point2i(width, height));
}

static int32 index = Sample::Register("material", MaterialTest);
