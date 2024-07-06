#include "../samples.h"
#include "bulbit/camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"

std::unique_ptr<Camera> CornellBoxLucy(Scene& scene)
{
    // Materials
    auto red = scene.CreateMaterial<DiffuseMaterial>(Spectrum(.65f, .05f, .05f));
    auto green = scene.CreateMaterial<DiffuseMaterial>(Spectrum(.12f, .45f, .15f));
    auto blue = scene.CreateMaterial<DiffuseMaterial>(Spectrum(.22f, .23f, .75f));
    auto white = scene.CreateMaterial<DiffuseMaterial>(Spectrum(.73f, .73f, .73f));
    auto wakgood_texture = ColorImageTexture::Create("res/wakdu.jpg");
    auto wakgood_mat = scene.CreateMaterial<DiffuseMaterial>(wakgood_texture);
    auto light = scene.CreateMaterial<DiffuseLightMaterial>(Spectrum(15));

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
        Transform transform{ Point3(0.5f, 0.0f, -0.5f), identity, Vec3(0.85f) };
        auto mat = scene.CreateMaterial<UnrealMaterial>(ConstantColorTexture::Create(1.0f), ConstantFloatTexture::Create(1.0f),
                                                        ConstantFloatTexture::Create(0.2f));

        // auto mat = scene.CreateMaterial<Dielectric>(1.5f);

        SetLoaderFallbackMaterial(mat);
        LoadModel(scene, "res/stanford/lucy.obj", transform);
    }

    int32 width = 500;

    Point3 lookfrom{ 0.5f, 0.5f, 1.25f };
    Point3 lookat{ 0.5f, 0.5f, 0 };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0;
    Float vFov = 45;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, width, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("cornell-box-lucy", CornellBoxLucy);
