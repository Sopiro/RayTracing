#include "../samples.h"
#include "bulbit/camera.h"
#include "bulbit/scene.h"

// 9967214 triangles
std::unique_ptr<Camera> SanMiguel(Scene& scene)
{
    Transform tf{ Vec3::zero, identity, Vec3(1) };
    LoadModel(scene, "res/San_Miguel/san-miguel.obj", tf);

    // scene.CreateLight<InfiniteAreaLight>("res/HDR/kloppenheim_07_puresky_1k.hdr");
    // scene.CreateLight<InfiniteAreaLight>("res/HDR/symmetrical_garden_1k.hdr");
    // scene.CreateLight<InfiniteAreaLight>("res/HDR/pizzo_pernice_1k.hdr");
    // scene.CreateLight<InfiniteAreaLight>("res/HDR/harties_4k.hdr");
    scene.CreateLight<InfiniteAreaLight>("res/HDR/quarry_04_puresky_1k.hdr");
    // scene.CreateLight<InfiniteAreaLight>("res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(-pi / 2, y_axis)));
    // scene.CreateLight<DirectionalLight>(Quat(DegToRad(119), -x_axis) * Vec3(0, 0, -1), 15 * Vec3(0.734f, 0.583f, 0.377f),
    // 0.02f);
    Spectrum sky_color(147 / 255.0f, 209 / 255.0f, 255 / 255.0f);
    scene.CreateLight<DirectionalLight>(Normalize(-Vec3(2.0f, 10.0f, 3.f)), Spectrum(25.0f), 0.01f);

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 1920;
    int32 height = int32(width / aspect_ratio);

    // Point3 lookfrom{ 6.5f, 2, 7.3f };
    // Point3 lookat{ 8, 2, 6.8f };

    // Point3 lookfrom{ 6.5f, 2, 2.0f };
    // Point3 lookat{ 8, 2, 3.0f };

    Point3 lookfrom{ 22.0f, 2, 5.0f };
    Point3 lookat{ 21.0f, 2, 4.5f };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 71;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("sanmiguel", SanMiguel);
