#include "../samples.h"
#include "bulbit/perspective_camera.h"
#include "bulbit/scene.h"

namespace bulbit
{

// https://developer.nvidia.com/orca/amazon-lumberyard-bistro
Camera* BistroScene(Scene& scene)
{
    Transform tf{ zero_vec3, identity, Vec3(1) };
    Ref<Model> m = CreateSharedRef<Model>("res/bistro/bistro.gltf", tf);

    scene.Add(m);

    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/kloppenheim_07_puresky_1k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/pizzo_pernice_1k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/harties_4k.hdr"));
    scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/quarry_04_puresky_1k.hdr"));
    scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(-pi / 2, y_axis))));
    // scene.AddLight(
    //     CreateSharedRef<DirectionalLight>(Quat(DegToRad(119), -x_axis) * Vec3(0, 0, -1), 15 * Vec3(0.734, 0.583, 0.377),
    //     0.02));

    Float aspect_ratio = 16.0f / 9.0f;
    // Float aspect_ratio = 3.0f / 2.0f;
    // Float aspect_ratio = 4.0f / 3.0f;
    // Float aspect_ratio = 1.0f;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ -21, 6, 0 };
    Point3 lookat{ 0, 1, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 54;

    return new PerspectiveCamera(lookfrom, lookat, y_axis, vFov, width, height, aperture, dist_to_focus);
}

static int32 index = Sample::Register("bistro", BistroScene);

} // namespace bulbit