#include "../samples.h"

std::unique_ptr<Camera> CornellBoxLucy(Scene& scene)
{
    // Materials
    auto red = CreateDiffuseMaterial(scene, Spectrum(.65f, .05f, .05f));
    auto green = CreateDiffuseMaterial(scene, Spectrum(.12f, .45f, .15f));
    auto blue = CreateDiffuseMaterial(scene, Spectrum(.22f, .23f, .75f));
    auto white = CreateDiffuseMaterial(scene, Spectrum(.73f, .73f, .73f));
    auto wakgood_texture = scene.CreateImageTexture<Spectrum>(ReadImage3("res/wakdu.jpg"));
    auto wakgood_mat = scene.CreateMaterial<DiffuseMaterial>(wakgood_texture);
    auto light = CreateDiffuseLightMaterial(scene, Spectrum(15));

    // Cornell box
    {
        // front
        auto tf = Transform{ Vec3(0.5f, 0.5f, -1.0f), identity, Vec3(1.0f) };
        CreateRectXY(scene, tf, wakgood_mat);

        // left
        tf = Transform{ Vec3(0.0f, 0.5f, -0.5f), identity, Vec3(1.0f) };
        CreateRectYZ(scene, tf, red);

        // right
        tf = Transform{ Vec3(1.0f, 0.5f, -0.5f), Quat(pi, y_axis), Vec3(1.0f) };
        CreateRectYZ(scene, tf, green);

        // bottom
        tf = Transform{ Vec3(0.5f, 0.0f, -0.5f), identity, Vec3(1.0f) };
        CreateRectXZ(scene, tf, white);

        // top
        tf = Transform{ Vec3(0.5f, 1.0f, -0.5f), Quat(pi, x_axis), Vec3(1.0f) };
        CreateRectXZ(scene, tf, white);
    }

    // Lights
    {
        auto tf = Transform{ 0.5f, 0.999f, -0.5f, Quat(pi, x_axis), Vec3(0.25f) };
        CreateRectXZ(scene, tf, light);
    }

    {
        // Lucy
        Transform transform{ Point3(0.5f, 0.0f, -0.5f), identity, Vec3(0.7f) };
        // auto mat = scene.CreateMaterial<UnrealMaterial>(
        //     scene.CreateConstantTexture<Spectrum>(1.0f), scene.CreateConstantTexture<Float>(1.0f),
        //     scene.CreateConstantTexture<Float>(0.2f)
        // );

        Spectrum sigma_a(0);
        Spectrum sigma_s(20, 100, 200);

        auto diffusion = CreateSubsurfaceMaterialDiffusion(scene, Spectrum(1.0), Spectrum(1) / (sigma_a + sigma_s), 1.0f, 0.0f);
        auto random_walk =
            CreateSubsurfaceMaterialRandomWalk(scene, Spectrum(1.0), Spectrum(1) / (sigma_a + sigma_s), 1.0f, 0.0f);
        auto mat = random_walk;
        HomogeneousMedium* hm = scene.CreateMedium<HomogeneousMedium>(sigma_a, sigma_s, Spectrum(0.0), 0.0f);

        MediumInterface mi_outside(nullptr, nullptr);
        MediumInterface mi_inside(hm, nullptr);
        MediumInterface mi_two_sided(nullptr, nullptr);

        // auto mat = scene.CreateMaterial<Dielectric>(1.5f);

        // SetLoaderFallbackMaterial(mat)
        SetLoaderUseForceFallbackMaterial(true);
        SetLoaderFallbackMaterial(mat);
        // SetLoaderFallbackMediumInterface(mi_inside);
        LoadModel(scene, "res/stanford/bunny.obj", transform);
    }

    int32 width = 500;

    Point3 lookfrom{ 0.5f, 0.5f, 2.05f };
    Point3 lookat{ 0.5f, 0.5f, 0.0f };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0.0f;
    Float vFov = 28.0f;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus, Point2i(width, width));
}

static int32 index = Sample::Register("cornell-box-lucy", CornellBoxLucy);
