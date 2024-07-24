#include "../samples.h"

std::unique_ptr<Camera> CornellBoxVolume(Scene& scene)
{
    // Materials
    auto red = scene.CreateMaterial<DiffuseMaterial>(Spectrum(.65f, .05f, .05f));
    auto green = scene.CreateMaterial<DiffuseMaterial>(Spectrum(.12f, .45f, .15f));
    auto blue = scene.CreateMaterial<DiffuseMaterial>(Spectrum(.22f, .23f, .75f));
    auto white = scene.CreateMaterial<DiffuseMaterial>(Spectrum(.73f, .73f, .73f));
    auto wakgood_texture = ColorImageTexture::Create("res/wakdu.jpg");
    auto wakgood_mat = scene.CreateMaterial<DiffuseMaterial>(wakgood_texture);
    auto light = scene.CreateMaterial<DiffuseLightMaterial>(Spectrum(2.0f));
    auto plastic = scene.CreateMaterial<DielectricMaterial>(1.5f, ConstantFloatTexture::Create(0.2f));
    // auto light = scene.CreateMaterial<DiffuseLightMaterial>(Spectrum(17.0f, 12.0f, 4.0f));
    auto mirror = scene.CreateMaterial<MirrorMaterial>(Spectrum(0.73f));
    auto mix = scene.CreateMaterial<MixtureMaterial>(red, blue, 0.5f);

    HomogeneousMedium* hm = scene.CreateMedium<HomogeneousMedium>(Spectrum(1), Spectrum(50), Spectrum(0.0), -0.9f);
    MediumInterface mi_outside(nullptr, nullptr);
    MediumInterface mi_inside(hm, nullptr);
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
        CreateBox(scene, tf, nullptr, mi_inside);
    }

    // Right block
    {
        Float hx = 0.14f;
        Float hy = 0.14f;
        Float hz = 0.14f;

        // auto mat = scene.CreateMaterial<ThinDielectricMaterial>(1.5f);

        auto tf = Transform{ 0.66f, hy + Ray::epsilon * 2, -0.33f, Quat(DegToRad(-18.0f), y_axis),
                             Vec3(hx * 2.0f, hy * 2.0f, hz * 2.0f) };
        CreateBox(scene, tf, nullptr, mi_inside);
    }

    // Right sphere
    {
        // auto mat = scene.CreateMaterial<DielectricMaterial>(1.5f, ConstantFloatTexture::Create(0.0f));
        // auto mat = scene.CreateMaterial<ThinDielectricMaterial>(1.5f);
        // auto mat = scene.CreateMaterial<ConductorMaterial>(
        //     ConstantColorTexture::Create(0.1, 0.2, 1.9), ConstantColorTexture::Create(3, 2.5, 2),
        //     ConstantFloatTexture::Create(0.3f), ConstantFloatTexture::Create(0.1f));
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
        // scene.CreateLight<ImageInfiniteLight>("res/HDR/quarry_04_puresky_1k.hdr");
        // scene.CreateLight<ImageInfiniteLight>("res/solitude_night_4k/solitude_night_4k.hdr");
        // scene.CreateLight<ImageInfiniteLight>("res/sunflowers/sunflowers_puresky_4k.hdr");
        // scene.CreateLight<ImageInfiniteLight>("res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(pi, y_axis)));
    }

    // std::cout << "Lights: " << scene.GetLights().size() << std::endl;

    int32 width = 500;

    Point3 lookfrom{ 0.5f, 0.5f, 1.9f };
    Point3 lookat{ 0.5f, 0.5f, 0.0f };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0.0f;
    Float vFov = 30.0f;

    // return std::make_unique<OrthographicCamera>(lookfrom, lookat, y_axis, 1.1, 1.1, width);
    // return std::make_unique<SphericalCamera>(lookfrom, width, width);
    return std::make_unique<PerspectiveCamera>(Vec2i(width, width), lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("cornell-box-volume", CornellBoxVolume);
