#include "../samples.h"
#include "bulbit/camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

std::unique_ptr<Camera> CameraScene(Scene& scene)
{
    // Floor
    {
        auto mat = scene.CreateMaterial<Microfacet>(ConstantColor::Create(Spectrum(0.5f)), ConstantFloatTexture::Create(0.0f),
                                                    ConstantFloatTexture::Create(0.01f));

        auto tf = Transform{ Vec3::zero, identity, Vec3(8.0f) };
        CreateRectXZ(scene, tf, mat);
    }

    // Camera
    {
        auto tf = Transform{ Vec3::zero, Quat(DegToRad(0.0f), y_axis), Vec3(0.11f) };
        LoadModel(scene, "res/AntiqueCamera/glTF/AntiqueCamera.gltf", tf);
    }

    // Lights
    {
        auto light = scene.CreateMaterial<DiffuseLight>(Spectrum(1.0f, 0.9f, 0.8f) * 3);
        Float w = 0.4f;
        Float h = 1.2f;
        auto tf = Transform{ Point3(1.0f, h / 2.0f - 0.01f, 0.0f), Quat(pi, y_axis), Vec3(1.0f, h, w) };
        CreateRectYZ(scene, tf, light);

        tf = Transform{ Point3(0.0f, h / 2.0f - 0.01f, -1.0f), Quat(0.0f, y_axis), Vec3(w, h, 1.0f) };
        CreateRectXY(scene, tf, light);

        tf = Transform{ Point3(0.0f, h / 2.0f - 0.01f, 1.0f), Quat(pi, y_axis), Vec3(w, h, 1.0f) };
        CreateRectXY(scene, tf, light);

        tf = Transform{ Point3(-1.0f, h / 2.0f - 0.01f, 0.0f), Quat(0.0f, y_axis), Vec3(1.0f, h, w) };
        CreateRectYZ(scene, tf, light);
    }

    // scene.SetEnvironmentMap(ImageTexture::Create("res/sunflowers/sunflowers_puresky_4k.hdr", false, true));

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ -2, 1, 2 };
    Point3 lookat{ 0, 0.5f, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 30;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("camera", CameraScene);
