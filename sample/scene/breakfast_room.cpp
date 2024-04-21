#include "../samples.h"
#include "bulbit/camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

// https://casual-effects.com/data/
std::unique_ptr<Camera> BreakfastRoom(Scene& scene)
{
    Transform tf{ Vec3::zero, Quat(DegToRad(0.0f), y_axis), Vec3(1.0f) };
    LoadModel(scene, "res/breakfast_room/breakfast_room.obj", tf);

    // scene.CreateLight<InfiniteAreaLight>("res/HDR/kloppenheim_07_puresky_1k.hdr");
    scene.CreateLight<InfiniteAreaLight>("res/HDR/quarry_04_puresky_1k.hdr");
    // scene.CreateLight<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr");

    scene.CreateLight<DirectionalLight>(Normalize(-Vec3(15.0f, 5.0f, 5.0f)), Vec3(8.0f), 0.02f);

    // auto light = scene.CreateMaterial<DiffuseLight>(Spectrum(5000.0));
    // tf = Transform{ 30, 10, 15, Quat(pi, z_axis), Vec3(1.0, 1.5, 5.0) };
    // auto l = CreateRectYZ(tf, light);

    // scene.AddLight(l);

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0, 2.2f, 4.5f };
    Point3 lookat{ 0, 1.5f, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 71;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("breakfast-room", BreakfastRoom);
