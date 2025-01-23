#include "../samples.h"

#include <typeinfo>

std::unique_ptr<Camera> CornellBoxVolume2(Scene& scene)
{
    // Materials
    auto red = CreateDiffuseMaterial(scene, Spectrum(.65f, .05f, .05f));
    auto green = CreateDiffuseMaterial(scene, Spectrum(.12f, .45f, .15f));
    auto blue = CreateDiffuseMaterial(scene, Spectrum(.22f, .23f, .75f));
    auto white = CreateDiffuseMaterial(scene, Spectrum(.73f, .73f, .73f));
    auto wakgood_mat = CreateDiffuseMaterial(scene, "res/wakdu.jpg");
    auto light = CreateDiffuseLightMaterial(scene, Spectrum(300.0f));
    auto glass = CreateDielectricMaterial(scene, 1.5f, 0.0f);
    auto plastic = CreateDielectricMaterial(scene, 1.5f, 0.2f);
    // auto light = CreateDiffuseLightMaterial(scene, Spectrum(17.0f, 12.0f, 4.0f));
    auto mirror = CreateMirrorMaterial(scene, Spectrum(0.73f));
    auto mix = CreateMixtureMaterial(scene, red, blue, 0.5f);

    Medium* hm = scene.CreateMedium<HomogeneousMedium>(Spectrum(0), Spectrum(0.5f), Spectrum(0.0), -0.7f);
    MediumInterface mi_outside(nullptr, hm);
    MediumInterface mi_inside(hm, nullptr);
    MediumInterface mi_two_sided(hm, hm);

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

    {
        // auto tf = Transform(Vec3(0.33f, 0.5f, -0.5f));
        // CreateSphere(scene, tf, 0.1f, glass);
    }

    {
        auto tf = Transform(Vec3(0.4f, 0.6f, -0.4f));
        CreateSphere(scene, tf, 0.15f, glass);
    }

    // Lights
    {
        // auto tf = Transform{ 0.5f, 0.995f, -0.5f, Quat(pi, x_axis), Vec3(0.08f) };
        // CreateRectXZ(scene, tf, light, mi_two_sided);

        CreateSphere(scene, Vec3(0.5f, 0.98f, -0.5f), 0.02f, light);
        // scene.CreateLight<PointLight>(Point3(0.5f, 1.0f - Ray::epsilon, -0.5f), Spectrum(0.2f));
        // scene.CreateLight<DirectionalLight>(Normalize(-Vec3(1, 1, 1)), Vec3(5.0f), 0.05f);
        // CreateImageInfiniteLight(scene, "res/HDR/quarry_04_puresky_1k.hdr");
        // CreateImageInfiniteLight(scene, "res/solitude_night_4k/solitude_night_4k.hdr");
        // CreateImageInfiniteLight(scene, "res/sunflowers/sunflowers_puresky_4k.hdr");
        // CreateImageInfiniteLight(scene, "res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(pi, y_axis)));
    }

    // std::cout << "Lights: " << scene.GetLights().size() << std::endl;

    int32 width = 1000;

    Point3 lookfrom{ 0.5f, 0.5f, 2.05f };
    Point3 lookat{ 0.5f, 0.5f, 0.0f };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0.0f;
    Float vFov = 28.0f;

    // return std::make_unique<OrthographicCamera>(lookfrom, lookat, y_axis, Point2(1.1, 1.1), width);
    // return std::make_unique<SphericalCamera>(lookfrom, Point2i(width, width));
    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus, Point2i(width, width));
}

static int32 index = Sample::Register("cornell-box-volume2", CornellBoxVolume2);
