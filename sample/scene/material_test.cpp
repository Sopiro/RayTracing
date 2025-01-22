#include "../samples.h"

std::unique_ptr<Camera> MaterialTest(Scene& scene)
{
    HomogeneousMedium* hm = scene.CreateMedium<HomogeneousMedium>(Spectrum(0, 0, 0), Spectrum(10), Spectrum(0.0), -0.9f);
    MediumInterface mi(hm, nullptr);

    SetLoaderUseForceFallbackMaterial(true);

    // Floor
    {
        auto a = scene.CreateConstantTexture<Spectrum>({ 0.75, 0.75, 0.75 });
        auto b = scene.CreateConstantTexture<Spectrum>({ 0.3, 0.3, 0.3 });
        auto checker = scene.CreateCheckerTexture<Spectrum>(a, b, Point2(20));
        auto tf = Transform{ Vec3(0, 0, 0), Quat::FromEuler({ 0, 0, 0 }), Vec3(3) };
        auto floor = scene.CreateMaterial<DiffuseMaterial>(checker);
        SetLoaderFallbackMaterial(floor);
        LoadModel(scene, "res/background.obj", tf);
    }

    Float scale = 2.0f;

    int32 w = 3;
    int32 h = 1;

    Vec3 o(0, 0, 0.4);
    Vec3 x(0.8, 0, 0);
    Vec3 y(0, 0, 1);

    const int32 count = 3;

    auto normalmap = scene.CreateImageTexture<Spectrum>(ReadImage3("res/bistro/Concrete_Normal.png", true));

    const Material* outers[count];
    outers[0] = CreateDielectricMaterial(scene, 1.5f, 0.08f);
    outers[1] = scene.CreateMaterial<ConductorMaterial>(
        scene.CreateConstantTexture<Spectrum>({ 0.1, 0.2, 1.9 }), scene.CreateConstantTexture<Spectrum>({ 3, 2.5, 2 }),
        scene.CreateConstantTexture<Float>(0.05f), scene.CreateConstantTexture<Float>(0.4f), normalmap
    );
    outers[2] = scene.CreateMaterial<UnrealMaterial>(
        scene.CreateConstantTexture<Spectrum>({ 80 / 255.0, 1.0, 175 / 255.0 }), scene.CreateConstantTexture<Float>(0),
        scene.CreateConstantTexture<Float>(0)
    );

    const Material* inners[count];
    inners[0] = scene.CreateMaterial<ConductorMaterial>(
        scene.CreateConstantTexture<Spectrum>(Spectrum{ 0.7f }), scene.CreateConstantTexture<Float>(0.05f),
        scene.CreateConstantTexture<Float>(0.4f)
    );
    inners[1] = CreateMirrorMaterial(scene, Spectrum(0.7f));
    inners[2] = CreateDiffuseMaterial(scene, Spectrum(0.7));

    for (int32 j = 0; j < h; ++j)
    {
        for (int32 i = 0; i < w; ++i)
        {
            int32 sign_i = std::pow<int32>(-1, i);
            int32 sign_j = std::pow<int32>(-1, j);
            Vec3 p = o + (sign_j * y * ((j + 1) / 2)) + (sign_i * x * ((i + 1) / 2));

            // SetLoaderFallbackMediumInterface(mi);

            auto tf = Transform{ p, Quat::FromEuler({ 0, pi / 6, 0 }), Vec3(scale) };

            // https://github.com/lighttransport/lighttransportequation-orb
            SetLoaderFallbackMaterial(outers[std::min(i + j * w, count)]);
            LoadModel(scene, "res/mori_knob/base.obj", tf);
            LoadModel(scene, "res/mori_knob/outer.obj", tf);

            SetLoaderFallbackMaterial(inners[std::min(i + j * w, count)]);
            LoadModel(scene, "res/mori_knob/inner.obj", tf);
            LoadModel(scene, "res/mori_knob/equation.obj", tf);
        }
    }

    // CreateImageInfiniteLight(scene,
    //     "res/material_test_ball/envmap.hdr", Transform(Quat::FromEuler(0, DegToRad(-67.26139831542969), 0))
    // );
    // CreateImageInfiniteLight(scene, "res/HDR/photo_studio_loft_hall_1k.hdr", Transform(Quat(pi, y_axis)));
    CreateImageInfiniteLight(scene, "res/HDR/aerodynamics_workshop_1k.hdr", Transform(Quat(pi, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/photo_studio_01_1k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/scythian_tombs_2_4k.hdr", Transform(Quat(pi, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/material-test.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/peppermint_powerplant_4k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/quarry_04_puresky_1k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/solitude_night_1k.hdr");
    // CreateImageInfiniteLight(scene, "res/sunflowers/sunflowers_puresky_4k.hdr");
    // CreateImageInfiniteLight(scene, "res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(-pi, y_axis)));
    // scene.CreateLight<UniformInfiniteLight>(Spectrum(1));

    Float aspect_ratio = 21.f / 9.f;
    // Float aspect_ratio = 9.f / 16.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 1600;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom = Point3{ 0, 1.0, 2.28 };
    Point3 lookat = Point3{ 0.0, 0.1, 0.0 };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0.01f;
    Float vFov = 30.0;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus, Point2i(width, height));
}

std::unique_ptr<Camera> Dielectrics(Scene& scene)
{
    HomogeneousMedium* hm = scene.CreateMedium<HomogeneousMedium>(Spectrum(0, 0, 0), Spectrum(10), Spectrum(0.0), -0.9f);
    MediumInterface mi(hm, nullptr);

    SetLoaderUseForceFallbackMaterial(true);

    // Floor
    {
        auto a = scene.CreateConstantTexture<Spectrum>({ 0.75, 0.75, 0.75 });
        auto b = scene.CreateConstantTexture<Spectrum>({ 0.3, 0.3, 0.3 });
        auto checker = scene.CreateCheckerTexture<Spectrum>(a, b, Point2(20));
        auto tf = Transform{ Vec3(0, 0, 0), Quat::FromEuler({ 0, 0, 0 }), Vec3(3) };
        auto floor = scene.CreateMaterial<DiffuseMaterial>(checker);
        SetLoaderFallbackMaterial(floor);
        LoadModel(scene, "res/background.obj", tf);
    }

    Float scale = 2.0f;

    int32 w = 3;
    int32 h = 1;

    Vec3 o(0, 0, 0.4);
    Vec3 x(0.8, 0, 0);
    Vec3 y(0, 0, 1);

    const int32 count = 3;

    auto normalmap = scene.CreateImageTexture<Spectrum>(ReadImage3("res/bistro/Concrete_Normal.png", true));

    const Material* outers[count];
    outers[0] = CreateDielectricMaterial(scene, 1.5f, 0.02f);
    outers[1] = CreateDielectricMaterial(scene, 1.5f, 0.0f);
    outers[2] = CreateDielectricMaterial(scene, 1.5f, 0.05f);

    const Material* inners[count];
    inners[0] = scene.CreateMaterial<UnrealMaterial>(
        scene.CreateConstantTexture<Spectrum>(Spectrum{ 0.66 }), scene.CreateConstantTexture<Float>(0),
        scene.CreateConstantTexture<Float>(0)
    );
    inners[1] = inners[0];
    inners[2] = inners[0];

    for (int32 j = 0; j < h; ++j)
    {
        for (int32 i = 0; i < w; ++i)
        {
            int32 sign_i = std::pow<int32>(-1, i);
            int32 sign_j = std::pow<int32>(-1, j);
            Vec3 p = o + (sign_j * y * ((j + 1) / 2)) + (sign_i * x * ((i + 1) / 2));

            // SetLoaderFallbackMediumInterface(mi);

            auto tf = Transform{ p, Quat::FromEuler({ 0, pi / 6, 0 }), Vec3(scale) };

            // https://github.com/lighttransport/lighttransportequation-orb
            SetLoaderFallbackMaterial(outers[std::min(i + j * w, count)]);
            LoadModel(scene, "res/mori_knob/base.obj", tf);
            LoadModel(scene, "res/mori_knob/outer.obj", tf);

            SetLoaderFallbackMaterial(inners[std::min(i + j * w, count)]);
            LoadModel(scene, "res/mori_knob/inner.obj", tf);
            LoadModel(scene, "res/mori_knob/equation.obj", tf);
        }
    }

    // CreateImageInfiniteLight(scene,
    //     "res/material_test_ball/envmap.hdr", Transform(Quat::FromEuler(0, DegToRad(-67.26139831542969), 0))
    // );
    // CreateImageInfiniteLight(scene, "res/HDR/photo_studio_loft_hall_1k.hdr", Transform(Quat(pi, y_axis)));
    CreateImageInfiniteLight(scene, "res/HDR/aerodynamics_workshop_1k.hdr", Transform(Quat(pi, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/photo_studio_01_1k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/scythian_tombs_2_4k.hdr", Transform(Quat(pi, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/material-test.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/peppermint_powerplant_4k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/quarry_04_puresky_1k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/solitude_night_1k.hdr");
    // CreateImageInfiniteLight(scene, "res/sunflowers/sunflowers_puresky_4k.hdr");
    // CreateImageInfiniteLight(scene, "res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(-pi, y_axis)));
    // scene.CreateLight<UniformInfiniteLight>(Spectrum(1));

    Float aspect_ratio = 21.f / 9.f;
    // Float aspect_ratio = 9.f / 16.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 1600;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom = Point3{ 0, 1.0, 2.28 };
    Point3 lookat = Point3{ 0.0, 0.1, 0.0 };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0.01f;
    Float vFov = 30.0;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus, Point2i(width, height));
}

std::unique_ptr<Camera> Skins(Scene& scene)
{
    HomogeneousMedium* hm = scene.CreateMedium<HomogeneousMedium>(Spectrum(0, 0, 0), Spectrum(10), Spectrum(0.0), -0.9f);
    MediumInterface mi(hm, nullptr);

    SetLoaderUseForceFallbackMaterial(true);

    // Floor
    {
        auto a = scene.CreateConstantTexture<Spectrum>({ 0.75, 0.75, 0.75 });
        auto b = scene.CreateConstantTexture<Spectrum>({ 0.3, 0.3, 0.3 });
        auto checker = scene.CreateCheckerTexture<Spectrum>(a, b, Point2(20));
        auto tf = Transform{ Vec3(0, 0, 0), Quat::FromEuler({ 0, 0, 0 }), Vec3(3) };
        auto floor = scene.CreateMaterial<DiffuseMaterial>(checker);
        SetLoaderFallbackMaterial(floor);
        LoadModel(scene, "res/background.obj", tf);
    }

    Float scale = 2.0f;

    int32 w = 3;
    int32 h = 1;

    Vec3 o(0, 0, 0.4);
    Vec3 x(0.8, 0, 0);
    Vec3 y(0, 0, 1);

    const int32 count = 3;

    auto normalmap = scene.CreateImageTexture<Spectrum>(ReadImage3("res/bistro/Concrete_Normal.png", true));

    const Material* skins[count];
    skins[1] = CreateSubsurfaceMaterialDiffusion(
        scene, Spectrum(255 / 255.0, 195 / 255.0, 170 / 255.0) * 0.8, Spectrum(0.5, 0.25, 0.125) * 0.05, 1.5f, 0.05f
    );
    skins[0] = CreateSubsurfaceMaterialDiffusion(
        scene, Spectrum(255 / 255.0, 195 / 255.0, 170 / 255.0) * 0.55, Spectrum(0.5, 0.25, 0.125) * 0.05, 1.5f, 0.05f
    );
    skins[2] = CreateSubsurfaceMaterialDiffusion(
        scene, Spectrum(255 / 255.0, 195 / 255.0, 170 / 255.0) * 0.1, Spectrum(0.5, 0.25, 0.125) * 0.05, 1.5f, 0.05f
    );

    for (int32 j = 0; j < h; ++j)
    {
        for (int32 i = 0; i < w; ++i)
        {
            int32 sign_i = std::pow<int32>(-1, i);
            int32 sign_j = std::pow<int32>(-1, j);
            Vec3 p = o + (sign_j * y * ((j + 1) / 2)) + (sign_i * x * ((i + 1) / 2));

            // SetLoaderFallbackMediumInterface(mi);

            auto tf = Transform{ p, Quat::FromEuler({ 0, pi / 6, 0 }), Vec3(scale) };

            // https://github.com/lighttransport/lighttransportequation-orb
            SetLoaderFallbackMaterial(skins[std::min(i + j * w, count)]);
            LoadModel(scene, "res/mori_knob/base.obj", tf);
            LoadModel(scene, "res/mori_knob/outer.obj", tf);

            // SetLoaderFallbackMaterial(skins[std::min(i + j * w, count)]);
            // LoadModel(scene, "res/mori_knob/inner.obj", tf);
            // LoadModel(scene, "res/mori_knob/equation.obj", tf);
        }
    }

    // CreateImageInfiniteLight(scene,
    //     "res/material_test_ball/envmap.hdr", Transform(Quat::FromEuler(0, DegToRad(-67.26139831542969), 0))
    // );
    // CreateImageInfiniteLight(scene, "res/HDR/photo_studio_loft_hall_1k.hdr", Transform(Quat(pi, y_axis)));
    CreateImageInfiniteLight(scene, "res/HDR/aerodynamics_workshop_1k.hdr", Transform(Quat(pi, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/photo_studio_01_1k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/scythian_tombs_2_4k.hdr", Transform(Quat(pi, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/material-test.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/peppermint_powerplant_4k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/quarry_04_puresky_1k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/solitude_night_1k.hdr");
    // CreateImageInfiniteLight(scene, "res/sunflowers/sunflowers_puresky_4k.hdr");
    // CreateImageInfiniteLight(scene, "res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(-pi, y_axis)));
    // scene.CreateLight<UniformInfiniteLight>(Spectrum(1));

    auto light = CreateDiffuseLightMaterial(scene, Spectrum(5.0f));
    auto tf = Transform{ 0.0f, 0.8f, -0.5f, Quat::FromEuler({ 0, 0, 0 }), Vec3(3, 0.5, 1) };
    CreateRectXY(scene, tf, light);

    Float aspect_ratio = 21.f / 9.f;
    // Float aspect_ratio = 9.f / 16.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 1600;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom = Point3{ 0, 1.0, 2.28 };
    Point3 lookat = Point3{ 0.0, 0.1, 0.0 };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0.01f;
    Float vFov = 30.0;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus, Point2i(width, height));
}

std::unique_ptr<Camera> Mixtures(Scene& scene)
{
    HomogeneousMedium* hm = scene.CreateMedium<HomogeneousMedium>(Spectrum(0, 0, 0), Spectrum(10), Spectrum(0.0), -0.9f);
    MediumInterface mi(hm, nullptr);

    SetLoaderUseForceFallbackMaterial(true);

    // Floor
    {
        auto a = scene.CreateConstantTexture<Spectrum>({ 0.75, 0.75, 0.75 });
        auto b = scene.CreateConstantTexture<Spectrum>({ 0.3, 0.3, 0.3 });
        auto checker = scene.CreateCheckerTexture<Spectrum>(a, b, Point2(20));
        auto tf = Transform{ Vec3(0, 0, 0), Quat::FromEuler({ 0, 0, 0 }), Vec3(3) };
        auto floor = scene.CreateMaterial<DiffuseMaterial>(checker);
        SetLoaderFallbackMaterial(floor);
        LoadModel(scene, "res/background.obj", tf);
    }

    Float scale = 2.0f;

    int32 w = 3;
    int32 h = 1;

    Vec3 o(0, 0, 0.4);
    Vec3 x(0.8, 0, 0);
    Vec3 y(0, 0, 1);

    const int32 count = 3;

    auto normalmap = scene.CreateImageTexture<Spectrum>(ReadImage3("res/bistro/Concrete_Normal.png", true));

    const Material* outers[count];
    auto a = CreateDielectricMaterial(scene, 1.5f, 0.0f);
    auto b = scene.CreateMaterial<ConductorMaterial>(
        scene.CreateConstantTexture<Spectrum>({ 0.7, 0.3, 0.2 }), scene.CreateConstantTexture<Float>(0.1f)
    );
    auto checker = scene.CreateCheckerTexture<Float>(
        scene.CreateConstantTexture<Float>(0), scene.CreateConstantTexture<Float>(1), Point2(20)
    );
    outers[1] = scene.CreateMaterial<MixtureMaterial>(a, b, checker);

    auto c = scene.CreateMaterial<ConductorMaterial>(
        scene.CreateConstantTexture<Spectrum>(Spectrum{ 0.7f }), scene.CreateConstantTexture<Float>(0.05f),
        scene.CreateConstantTexture<Float>(0.4f)
    );
    auto d = scene.CreateMaterial<ConductorMaterial>(
        scene.CreateConstantTexture<Spectrum>(Spectrum{ 0.7f }), scene.CreateConstantTexture<Float>(0.4f),
        scene.CreateConstantTexture<Float>(0.05f)
    );
    outers[0] = scene.CreateMaterial<MixtureMaterial>(c, d, checker);

    auto e = CreateDiffuseMaterial(scene, Spectrum(0.7, 0.9, 0.5));
    auto f = CreateMirrorMaterial(scene, Spectrum(0.6, 0.5, 0.4));
    outers[2] = scene.CreateMaterial<MixtureMaterial>(e, f, checker);

    const Material* inners[count];
    inners[0] = scene.CreateMaterial<UnrealMaterial>(
        scene.CreateConstantTexture<Spectrum>(Spectrum{ 0.66 }), scene.CreateConstantTexture<Float>(0),
        scene.CreateConstantTexture<Float>(0)
    );
    inners[1] = inners[0];
    inners[2] = inners[0];

    for (int32 j = 0; j < h; ++j)
    {
        for (int32 i = 0; i < w; ++i)
        {
            int32 sign_i = std::pow<int32>(-1, i);
            int32 sign_j = std::pow<int32>(-1, j);
            Vec3 p = o + (sign_j * y * ((j + 1) / 2)) + (sign_i * x * ((i + 1) / 2));

            // SetLoaderFallbackMediumInterface(mi);

            auto tf = Transform{ p, Quat::FromEuler({ 0, pi / 6, 0 }), Vec3(scale) };

            // https://github.com/lighttransport/lighttransportequation-orb
            SetLoaderFallbackMaterial(outers[std::min(i + j * w, count)]);
            LoadModel(scene, "res/mori_knob/base.obj", tf);
            LoadModel(scene, "res/mori_knob/outer.obj", tf);

            SetLoaderFallbackMaterial(inners[std::min(i + j * w, count)]);
            LoadModel(scene, "res/mori_knob/inner.obj", tf);
            LoadModel(scene, "res/mori_knob/equation.obj", tf);
        }
    }

    // CreateImageInfiniteLight(scene,
    //     "res/material_test_ball/envmap.hdr", Transform(Quat::FromEuler(0, DegToRad(-67.26139831542969), 0))
    // );
    // CreateImageInfiniteLight(scene, "res/HDR/photo_studio_loft_hall_1k.hdr", Transform(Quat(pi, y_axis)));
    CreateImageInfiniteLight(scene, "res/HDR/aerodynamics_workshop_1k.hdr", Transform(Quat(pi, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/photo_studio_01_1k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/scythian_tombs_2_4k.hdr", Transform(Quat(pi, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/material-test.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/peppermint_powerplant_4k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/quarry_04_puresky_1k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/solitude_night_1k.hdr");
    // CreateImageInfiniteLight(scene, "res/sunflowers/sunflowers_puresky_4k.hdr");
    // CreateImageInfiniteLight(scene, "res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(-pi, y_axis)));
    // scene.CreateLight<UniformInfiniteLight>(Spectrum(1));

    Float aspect_ratio = 21.f / 9.f;
    // Float aspect_ratio = 9.f / 16.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 1600;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom = Point3{ 0, 1.0, 2.28 };
    Point3 lookat = Point3{ 0.0, 0.1, 0.0 };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0.01f;
    Float vFov = 30.0;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus, Point2i(width, height));
}

std::unique_ptr<Camera> MaterialTest5(Scene& scene)
{
    HomogeneousMedium* hm = scene.CreateMedium<HomogeneousMedium>(Spectrum(0, 0, 0), Spectrum(10), Spectrum(0.0), -0.9f);
    MediumInterface mi(hm, nullptr);

    SetLoaderUseForceFallbackMaterial(true);

    // Floor
    {
        auto a = scene.CreateConstantTexture<Spectrum>({ 0.75, 0.75, 0.75 });
        auto b = scene.CreateConstantTexture<Spectrum>({ 0.3, 0.3, 0.3 });
        auto checker = scene.CreateCheckerTexture<Spectrum>(a, b, Point2(20));
        auto tf = Transform{ Vec3(0, 0, 0), Quat::FromEuler({ 0, 0, 0 }), Vec3(3) };
        auto floor = scene.CreateMaterial<DiffuseMaterial>(checker);
        SetLoaderFallbackMaterial(floor);
        LoadModel(scene, "res/background.obj", tf);
    }

    Float scale = 2.0f;

    int32 w = 3;
    int32 h = 1;

    Vec3 o(0, 0, 0.4);
    Vec3 x(0.8, 0, 0);
    Vec3 y(0, 0, 1);

    const int32 count = 3;

    auto normalmap = scene.CreateImageTexture<Spectrum>(ReadImage3("res/bistro/Concrete_Normal.png", true));

    const Material* outers[count];
    outers[0] = CreateDiffuseMaterial(scene, Spectrum(.65f, .05f, .05f), nullptr, 0.4f);
    outers[1] = CreateDiffuseMaterial(scene, Spectrum(.12f, .45f, .15f), nullptr, 0.4f);
    outers[2] = CreateDiffuseMaterial(scene, Spectrum(.22f, .23f, .75f), nullptr, 0.4f);

    const Material* inners[count];
    inners[0] = scene.CreateMaterial<UnrealMaterial>(
        scene.CreateConstantTexture<Spectrum>(Spectrum{ 0.66 }), scene.CreateConstantTexture<Float>(0),
        scene.CreateConstantTexture<Float>(0)
    );
    inners[1] = inners[0];
    inners[2] = inners[0];

    for (int32 j = 0; j < h; ++j)
    {
        for (int32 i = 0; i < w; ++i)
        {
            int32 sign_i = std::pow<int32>(-1, i);
            int32 sign_j = std::pow<int32>(-1, j);
            Vec3 p = o + (sign_j * y * ((j + 1) / 2)) + (sign_i * x * ((i + 1) / 2));

            // SetLoaderFallbackMediumInterface(mi);

            auto tf = Transform{ p, Quat::FromEuler({ 0, pi / 6, 0 }), Vec3(scale) };

            // https://github.com/lighttransport/lighttransportequation-orb
            SetLoaderFallbackMaterial(outers[std::min(i + j * w, count)]);
            LoadModel(scene, "res/mori_knob/base.obj", tf);
            LoadModel(scene, "res/mori_knob/outer.obj", tf);

            SetLoaderFallbackMaterial(inners[std::min(i + j * w, count)]);
            LoadModel(scene, "res/mori_knob/inner.obj", tf);
            LoadModel(scene, "res/mori_knob/equation.obj", tf);
        }
    }

    // CreateImageInfiniteLight(scene,
    //     "res/material_test_ball/envmap.hdr", Transform(Quat::FromEuler(0, DegToRad(-67.26139831542969), 0))
    // );
    // CreateImageInfiniteLight(scene, "res/HDR/photo_studio_loft_hall_1k.hdr", Transform(Quat(pi, y_axis)));
    CreateImageInfiniteLight(scene, "res/HDR/aerodynamics_workshop_1k.hdr", Transform(Quat(pi, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/photo_studio_01_1k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/scythian_tombs_2_4k.hdr", Transform(Quat(pi, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/material-test.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/peppermint_powerplant_4k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/quarry_04_puresky_1k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/solitude_night_1k.hdr");
    // CreateImageInfiniteLight(scene, "res/sunflowers/sunflowers_puresky_4k.hdr");
    // CreateImageInfiniteLight(scene, "res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(-pi, y_axis)));
    // scene.CreateLight<UniformInfiniteLight>(Spectrum(1));

    Float aspect_ratio = 21.f / 9.f;
    // Float aspect_ratio = 9.f / 16.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 1600;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom = Point3{ 0, 1.0, 2.28 };
    Point3 lookat = Point3{ 0.0, 0.1, 0.0 };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0.01f;
    Float vFov = 30.0;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus, Point2i(width, height));
}

static int32 index = Sample::Register("material", MaterialTest);
static int32 index2 = Sample::Register("material2", Dielectrics);
static int32 index3 = Sample::Register("material3", Skins);
static int32 index4 = Sample::Register("material4", Mixtures);
static int32 index5 = Sample::Register("material5", MaterialTest5);
