#include "../samples.h"
#include "bulbit/diffuse_light.h"
#include "bulbit/lambertian.h"
#include "bulbit/perspective_camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

Camera* CameraScene(Scene& scene)
{
    // Floor
    {
        auto mat = CreateSharedRef<Microfacet>(ConstantColor::Create(Spectrum(0.5f)), ConstantColor::Create(Spectrum(0.0f)),
                                               ConstantColor::Create(Spectrum(0.01f)));

        auto tf = Transform{ zero_vec3, identity, Vec3(8.0f) };
        auto floor = CreateRectXZ(tf, mat);
        scene.Add(floor);
    }

    // Camera
    {
        auto tf = Transform{ zero_vec3, Quat(DegToRad(0.0f), y_axis), Vec3(0.11f) };
        auto model = CreateSharedRef<Model>("res/AntiqueCamera/glTF/AntiqueCamera.gltf", tf);

        scene.Add(model);
    }

    // Lights
    {
        auto light = CreateSharedRef<DiffuseLight>(Spectrum(1.0f, 0.9f, 0.8f) * 3);
        Float w = 0.4f;
        Float h = 1.2f;
        auto tf = Transform{ Point3(1.0f, h / 2.0f - 0.01f, 0.0f), Quat(pi, y_axis), Vec3(1.0f, h, w) };
        auto rect = CreateRectYZ(tf, light);

        scene.AddLight(rect);

        tf = Transform{ Point3(0.0f, h / 2.0f - 0.01f, -1.0f), Quat(0.0f, y_axis), Vec3(w, h, 1.0f) };
        rect = CreateRectXY(tf, light);

        scene.AddLight(rect);

        tf = Transform{ Point3(0.0f, h / 2.0f - 0.01f, 1.0f), Quat(pi, y_axis), Vec3(w, h, 1.0f) };
        rect = CreateRectXY(tf, light);

        scene.AddLight(rect);

        tf = Transform{ Point3(-1.0f, h / 2.0f - 0.01f, 0.0f), Quat(0.0f, y_axis), Vec3(1.0f, h, w) };
        rect = CreateRectYZ(tf, light);

        scene.AddLight(rect);
    }

    // scene.SetEnvironmentMap(ImageTexture::Create("res/sunflowers/sunflowers_puresky_4k.hdr", false, true));

    Float aspect_ratio = 16. / 9.;
    // Float aspect_ratio = 3. / 2.;
    // Float aspect_ratio = 4. / 3.;
    // Float aspect_ratio = 1.;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ -2, 1, 2 };
    Point3 lookat{ 0, 0.5f, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 30;

    return new PerspectiveCamera(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("camera", CameraScene);

} // namespace bulbit