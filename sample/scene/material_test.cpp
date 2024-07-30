#include "../samples.h"

std::unique_ptr<Camera> MaterialTest(Scene& scene)
{
    HomogeneousMedium* hm = scene.CreateMedium<HomogeneousMedium>(Spectrum(0, 0, 0), Spectrum(10), Spectrum(0.0), -0.9f);
    MediumInterface mi(hm, nullptr);

    SetLoaderUseForceFallbackMaterial(true);

    // Floor
    {
        auto a = ConstantColorTexture::Create(0.725, 0.71, 0.68);
        auto b = ConstantColorTexture::Create(0.325, 0.31, 0.25);
        auto checker = CheckerTexture::Create(a, b, Point2(20));
        auto tf = Transform{ Vec3(-0.708772, 0, -0.732108), Quat::FromEuler(0, DegToRad(46.1511), DegToRad(180)), Vec3(5.43618) };
        auto floor = scene.CreateMaterial<DiffuseMaterial>(checker);
        CreateRectXZ(scene, tf, floor);
    }

    // Model
    {
        // auto normalmap = ColorImageTexture::Create("res/bistro/Concrete_Normal.png");
        auto normalmap = nullptr;
        // auto mat = scene.CreateMaterial<DiffuseMaterial>(Spectrum(0.9), normalmap);
        auto mat = scene.CreateMaterial<ConductorMaterial>(
            ConstantColorTexture::Create(0.1, 0.2, 1.9), ConstantColorTexture::Create(3, 2.5, 2),
            ConstantFloatTexture::Create(0.05f), ConstantFloatTexture::Create(0.4f), normalmap
        );
        // auto mat = scene.CreateMaterial<DielectricMaterial>(1.5f, ConstantFloatTexture::Create(0.0f));
        // auto mat = scene.CreateMaterial<ThinDielectricMaterial>(1.5f);
        // auto mat = scene.CreateMaterial<MirrorMaterial>(Spectrum(0.7f), normalmap);

        // Srand(1213212);
        // auto mat = CreateRandomUnrealMaterial(scene);

        SetLoaderFallbackMaterial(mat);
        // SetLoaderFallbackMediumInterface(mi);

        auto tf = Transform{ Vec3(0.0571719, 0.213656, 0.0682078), Quat::FromEuler(0, 0, 0), Vec3(0.482906) };

        LoadModel(scene, "res/material_test_ball/models/Mesh001.obj", tf);
        tf = Transform{ Vec3(0.156382, 0.777229, 0.161698), Quat::FromEuler(0, 0, 0), Vec3(0.482906) };
        LoadModel(scene, "res/material_test_ball/models/Mesh002.obj", tf);
    }

    // Stand
    {
        auto mat = scene.CreateMaterial<DiffuseMaterial>(Spectrum(0.3));
        SetLoaderFallbackMaterial(mat);

        auto tf = Transform{ Vec3(0.110507, 0.494301, 0.126194), Quat::FromEuler(0, 0, 0), Vec3(0.482906) };
        LoadModel(scene, "res/material_test_ball/models/Mesh000.obj", tf);
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
    int32 width = 960;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 3.04068, 3.17153, 3.20454 };
    Point3 lookat{ 0.118789, 0.473398, 0.161081 };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0.0f;
    Float vFov = 20.114292;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus, Point2i(width, height));
}

static int32 index = Sample::Register("material", MaterialTest);
