#include "../samples.h"

// https://developer.nvidia.com/orca/amazon-lumberyard-bistro
std::unique_ptr<Camera> BistroScene(Scene& scene)
{
    // auto white = CreateDiffuseMaterial(scene, Spectrum(.73f, .73f, .73f));
    // SetLoaderFallbackMaterial(white);
    // SetLoaderUseForceFallbackMaterial(true);

    Transform tf{ Vec3::zero, identity, Vec3(0.01) };
    LoadModel(scene, "C:/Users/sopir/Desktop/bistro/bistro.gltf", tf);

    CreateImageInfiniteLight(scene, "res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(-pi / 2, y_axis)));
    CreateDirectionalLight(scene, Quat(DegToRad(119), -x_axis).Rotate(Vec3(0, 0, -1)), 15 * Vec3(0.734f, 0.583f, 0.377f), 0.02f);

    Float aspect_ratio = 16.f / 9.f;
    int32 width = 1600;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ -7.5, 1.5, 5 };
    Point3 lookat{ -4, 1, 0 };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0.02;
    Float vFov = 31;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus, Point2i(width, height));
}

static int32 index = Sample::Register("bistro", BistroScene);
