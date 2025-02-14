#include "../samples.h"

std::unique_ptr<Camera> BRDFSamplingTest(Scene& scene)
{
    // Materials
    auto red = CreateDiffuseMaterial(scene, Spectrum(.65f, .05f, .05f));
    auto green = CreateDiffuseMaterial(scene, Spectrum(.12f, .45f, .15f));
    auto blue = CreateDiffuseMaterial(scene, Spectrum(.22f, .23f, .75f));
    auto white = CreateDiffuseMaterial(scene, Spectrum(.73f, .73f, .73f));
    auto wakgood_mat = CreateDiffuseMaterial(scene, "res/wakdu.jpg");
    auto light = CreateDiffuseLightMaterial(scene, Spectrum(15.0));

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

    // Center sphere
    {
        auto mat = CreateUnrealMaterial(scene, Spectrum(1.0f), 1.0f, 0.2f);

        Float r = 0.25;
        CreateSphere(scene, Vec3(0.5f, r, -0.5f), r, mat);
    }

    // scene.Rebuild();
    // std::cout << scene.GetLights().GetCount() << std::endl;

    // Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    Float aspect_ratio = 1.f;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0.5f, 0.5f, 2.05f };
    Point3 lookat{ 0.5f, 0.5f, 0.0f };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0.0f;
    Float vFov = 28.0f;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus, Point2i(width, height));
}

static int32 sample_index = Sample::Register("brdf-sampling", BRDFSamplingTest);
