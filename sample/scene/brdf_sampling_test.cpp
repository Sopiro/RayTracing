#include "../samples.h"
#include "bulbit/camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

std::unique_ptr<Camera> BRDFSamplingTest(Scene& scene)
{
    // Materials
    auto red = scene.CreateMaterial<Lambertian>(Spectrum(.65f, .05f, .05f));
    auto green = scene.CreateMaterial<Lambertian>(Spectrum(.12f, .45f, .15f));
    auto blue = scene.CreateMaterial<Lambertian>(Spectrum(.22f, .23f, .75f));
    auto white = scene.CreateMaterial<Lambertian>(Spectrum(.73f, .73f, .73f));
    auto wakgood_texture = ImageTexture::Create("res/wakdu.jpg");
    auto wakgood_mat = scene.CreateMaterial<Lambertian>(wakgood_texture);
    auto light = scene.CreateMaterial<DiffuseLight>(Spectrum(15.0));

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
        auto mat = scene.CreateMaterial<Microfacet>(ConstantColor::Create(Spectrum(1.0f)), ConstantFloatTexture::Create(1.0f),
                                                    ConstantFloatTexture::Create(0.2f));

        Float r = 0.25;
        scene.CreatePrimitive<Sphere>(Vec3(0.5f, r, -0.5f), r, mat);
    }

    // scene.Rebuild();
    // std::cout << scene.GetLights().GetCount() << std::endl;

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0.5f, 0.5f, 1.25f };
    Point3 lookat{ 0.5f, 0.5f, 0.0f };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 45;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("brdf-sampling", BRDFSamplingTest);
