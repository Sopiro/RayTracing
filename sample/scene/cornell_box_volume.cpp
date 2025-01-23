#include "../samples.h"

std::unique_ptr<Camera> CornellBoxVolume(Scene& scene)
{
    // Materials
    auto red = CreateDiffuseMaterial(scene, Spectrum(.65f, .05f, .05f));
    auto green = CreateDiffuseMaterial(scene, Spectrum(.12f, .45f, .15f));
    auto blue = CreateDiffuseMaterial(scene, Spectrum(.22f, .23f, .75f));
    auto white = CreateDiffuseMaterial(scene, Spectrum(.73f, .73f, .73f));
    auto wakgood_mat = CreateDiffuseMaterial(scene, "res/wakdu.jpg");
    auto light = CreateDiffuseLightMaterial(scene, Spectrum(2.0f));
    auto plastic = CreateDielectricMaterial(scene, 1.5f, 0.2f);
    // auto light = CreateDiffuseLightMaterial(scene, Spectrum(17.0f, 12.0f, 4.0f));
    auto mirror = CreateMirrorMaterial(scene, Spectrum(0.73f));
    auto mix = CreateMixtureMaterial(scene, red, blue, 0.5f);

    Spectrum sigma_a(0);
    Spectrum sigma_s(20, 100, 200);
    // Spectrum sigma_s(100);

    auto diffusion = CreateSubsurfaceDiffusionMaterial(scene, Spectrum(1.0), Spectrum(1) / (sigma_a + sigma_s), 1.0f, 0.0f);
    auto random_walk = CreateSubsurfaceRandomWalkMaterial(scene, Spectrum(1.0), Spectrum(1) / (sigma_a + sigma_s), 1.0f, 0.0f);

    auto boundary = CreateDielectricMaterial(scene, 1.0f);
    auto mat = random_walk;
    HomogeneousMedium* hm = scene.CreateMedium<HomogeneousMedium>(sigma_a, sigma_s, Spectrum(0.0), 0.0f);

    MediumInterface mi_outside(nullptr, nullptr);
    MediumInterface mi_inside(nullptr, nullptr);
    MediumInterface mi_two_sided(nullptr, nullptr);

    // Cornell box
    {
        // front
        auto tf = Transform{ Vec3(0.5f, 0.5f, -1.0f), identity, Vec3(1.0f) };
        CreateRectXY(scene, tf, wakgood_mat, mi_outside);

        // left
        tf = Transform{ Vec3(0.0f, 0.5f, -0.5f), identity, Vec3(1.0f) };
        CreateRectYZ(scene, tf, red, mi_outside);

        // right
        tf = Transform{ Vec3(1.0f, 0.5f, -0.5f), Quat(pi, y_axis), Vec3(1.0f) };
        CreateRectYZ(scene, tf, green, mi_outside);

        // bottom
        tf = Transform{ Vec3(0.5f, 0.0f, -0.5f), identity, Vec3(1.0f) };
        CreateRectXZ(scene, tf, white, mi_outside);

        // top
        tf = Transform{ Vec3(0.5f, 1.0f, -0.5f), Quat(pi, x_axis), Vec3(1.0f) };
        CreateRectXZ(scene, tf, white, mi_outside);

        // back
        tf = Transform{ Vec3(0.5f, 0.5f, 0.0f), Quat(pi, y_axis), Vec3(1.0f) };
        CreateRectXY(scene, tf, nullptr, mi_outside);
    }

    // Left block
    {
        Float hx = 0.14f;
        Float hy = 0.28f;
        Float hz = 0.14f;

        auto tf = Transform{ 0.33f, hy + Ray::epsilon * 2, -0.66f, Quat(DegToRad(18.0f), y_axis),
                             Vec3(hx * 2.0f, hy * 2.0f, hz * 2.0f) };
        CreateBox(scene, tf, mat, mi_inside);
    }

    // Right block
    {
        Float hx = 0.14f;
        Float hy = 0.14f;
        Float hz = 0.14f;

        // auto mat = scene.CreateMaterial<ThinDielectricMaterial>(1.5f);

        auto tf = Transform{ 0.66f, hy + Ray::epsilon * 2, -0.33f, Quat(DegToRad(-18.0f), y_axis),
                             Vec3(hx * 2.0f, hy * 2.0f, hz * 2.0f) };
        CreateBox(scene, tf, mat, mi_inside);
    }

    // Right sphere
    {
        // auto mat = CreateDielectricMaterial(scene, 1.5f, CreateFloatConstantTexture(scene, 0.0f));
        // auto mat = scene.CreateMaterial<ThinDielectricMaterial>(1.5f);
        // auto mat = CreateConductorMaterial(scene, { 0.1, 0.2, 1.9 }, { 3, 2.5, 2 }, 0.3f, 0.1f);
        // CreateSphere(scene, Transform(Vec3(0.65f, 0.15f, -0.3f), Quat(DegToRad(0), x_axis)), 0.15f, mat);
        // CreateSphere(scene, Transform(Vec3(0.65f, 0.15f, -0.3f), Quat(DegToRad(45), y_axis)), 0.15f, wakgood_mat);
    }

    // Lights
    {
        auto tf = Transform{ 0.5f, 0.995f, -0.5f, Quat(pi, x_axis), Vec3(0.7f) };
        CreateRectXZ(scene, tf, light, mi_two_sided);

        // CreateSphere(scene, Vec3(0.5f, 0.9f, -0.5f), 0.05f, light);
        // scene.CreateLight<PointLight>(Point3(0.5f, 0.9f, -0.5f), Spectrum(0.25f));
        // scene.CreateLight<DirectionalLight>(Normalize(-Vec3(1, 1, 1)), Vec3(5.0f), 0.05f);
        // CreateImageInfiniteLight(scene, "res/HDR/quarry_04_puresky_1k.hdr");
        // CreateImageInfiniteLight(scene, "res/solitude_night_4k/solitude_night_4k.hdr");
        // CreateImageInfiniteLight(scene, "res/sunflowers/sunflowers_puresky_4k.hdr");
        // CreateImageInfiniteLight(scene, "res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(pi, y_axis)));
    }

    // std::cout << "Lights: " << scene.GetLights().size() << std::endl;

    int32 width = 500;

    Point3 lookfrom{ 0.5f, 0.5f, 2.05f };
    Point3 lookat{ 0.5f, 0.5f, 0.0f };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0.0f;
    Float vFov = 28.0f;

    // return std::make_unique<OrthographicCamera>(lookfrom, lookat, y_axis, Point2(1.1, 1.1), width);
    // return std::make_unique<SphericalCamera>(lookfrom, Point2i(width, width));
    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus, Point2i(width, width));
}

static int32 index = Sample::Register("cornell-box-volume", CornellBoxVolume);
