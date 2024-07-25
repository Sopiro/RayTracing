#include "../samples.h"

std::unique_ptr<Camera> NormalMappingTest(Scene& scene)
{
    // Transform transform{ Vec3::zero, Quat(DegToRad(0.0f), y_axis), Vec3(0.01f) };
    // Model model = Model("res/pbr_kabuto_samurai_helmet/scene.gltf", transform);

    Transform transform{ Vec3::zero, Quat(DegToRad(0.0f), y_axis), Vec3(1.0f) };
    LoadModel(scene, "res/DamagedHelmet/DamagedHelmet.gltf", transform);

    scene.CreateLight<ImageInfiniteLight>("res/HDR/peppermint_powerplant_4k.hdr");

    // scene.CreateLight<ImageInfiniteLight>("res/sunflowers/sunflowers_puresky_4k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/solitude_night_4k/solitude_night_4k.hdr");

    // scene.CreateLight<DirectionalLight>(Normalize(-Vec3(-1, 10, 0)), Vec3(1.0f), 0.01f);

    // Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3. / 2;
    // Float aspect_ratio = 4 / 3;
    Float aspect_ratio = 1.f;
    int32 width = 960;
    int32 height = int32(width / aspect_ratio);

    // Point3 lookfrom{ 10.0, 0.0, 10.0 };
    // Point3 lookat{ 3.0, -2.5, 1.0 };

    Point3 lookfrom = Point3(1, 0.5f, 4) * 1.2f;
    Point3 lookat = Point3(0, 0, 0);

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0;
    Float vFov = 30;

    return std::make_unique<PerspectiveCamera>(Point2i(width, height), lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus);
    // return std::make_unique<SphericalCamera>(lookfrom, width, height);
}

static int32 index = Sample::Register("normal-mapping", NormalMappingTest);
