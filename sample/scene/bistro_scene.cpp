#include "../samples.h"
#include "bulbit/camera.h"
#include "bulbit/scene.h"

// https://developer.nvidia.com/orca/amazon-lumberyard-bistro
std::unique_ptr<Camera> BistroScene(Scene& scene)
{
    Transform tf{ Vec3::zero, identity, Vec3(1) };
    LoadModel(scene, "res/bistro/bistro.gltf", tf);

    // scene.CreateLight<InfiniteAreaLight>("res/HDR/kloppenheim_07_puresky_1k.hdr");
    // scene.CreateLight<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr");
    // scene.CreateLight<InfiniteAreaLight>("res/HDR/pizzo_pernice_1k.hdr");
    // scene.CreateLight<InfiniteAreaLight>("res/HDR/harties_4k.hdr");
    scene.CreateLight<InfiniteAreaLight>("res/HDR/quarry_04_puresky_1k.hdr");
    scene.CreateLight<InfiniteAreaLight>("res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(-pi / 2, y_axis)));
    scene.CreateLight<DirectionalLight>(Quat(DegToRad(119), -x_axis) * Vec3(0, 0, -1), 15 * Vec3(0.734f, 0.583f, 0.377f), 0.02f);

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ -21, 6, 0 };
    Point3 lookat{ 0, 1, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 54;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("bistro", BistroScene);
