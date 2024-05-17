#include "../samples.h"
#include "bulbit/camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

std::unique_ptr<Camera> RebootScene(Scene& scene)
{
    // https://sketchfab.com/3d-models/reboot-dramatic-54ec601a3c4e4f6d8600fd28174c527c
    {
        auto mat = scene.CreateMaterial<UnrealMaterial>(ConstantColorTexture::Create(0.0f), ConstantFloatTexture::Create(0.0f),
                                                        ConstantFloatTexture::Create(0.001f));

        auto tf = Transform{ Vec3::zero, Quat(DegToRad(0.0f), y_axis), Vec3(0.01f) };
        LoadModel(scene, "res/reboot_dramatic_scene/scene.gltf", tf, mat);
    }

    {
        auto red = scene.CreateMaterial<DiffuseLight>(Spectrum(14.0f, 0.0f, 0.0f));
        auto s = scene.CreatePrimitive<Sphere>(Vec3(0.0f, 3.0f, -4.0f), 1.0f, red);
        scene.CreateLight<AreaLight>(s);
    }

    {
        auto white = scene.CreateMaterial<DiffuseLight>(Spectrum(8.0f));
        auto tf = Transform{ Vec3(0.0f, 8.0f, 0.0f), Quat(DegToRad(180.0f), x_axis), Vec3(3.0f) };
        CreateRectXZ(scene, tf, white, true);
    }

    // scene.CreateLight<ImageInfiniteLight>("res/solitude_night_4k/solitude_night_4k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/photo_studio_01_1k.hdr");

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ -4, 3.5f, -4 };
    Point3 lookat{ 0, 0, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0.02f;
    Float vFov = 30;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("reboot", RebootScene);
