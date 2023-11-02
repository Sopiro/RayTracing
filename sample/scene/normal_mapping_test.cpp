#include "../samples.h"
#include "bulbit/bulbit.h"
#include "bulbit/perspective_camera.h"

namespace bulbit
{

Camera* NormalMappingTest(Scene& scene)
{
    // Transform transform{ zero_vec3, Quat(DegToRad(0.0f), y_axis), Vec3(0.01f) };
    // Ref<Model> model = CreateSharedRef<Model>("res/pbr_kabuto_samurai_helmet/scene.gltf", transform);

    Transform transform{ zero_vec3, Quat(DegToRad(0.0f), y_axis), Vec3(1.0f) };
    Ref<Model> model = CreateSharedRef<Model>("res/DamagedHelmet/DamagedHelmet.gltf", transform);

    scene.Add(model);

    scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/peppermint_powerplant_4k.hdr"));

    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/solitude_night_4k/solitude_night_4k.hdr"));

    // scene.AddLight(CreateSharedRef<DirectionalLight>(Normalize(-Vec3(-1, 10, 0)), Vec3(1.0f), 0.01f));
    // scene.SetEnvironmentMap(ConstantColor::Create(zero_vec3));

    Float aspect_ratio = 1.0f;

    // Point3 lookfrom{ 10.0, 0.0, 10.0 };
    // Point3 lookat{ 3.0, -2.5, 1.0 };

    Point3 lookfrom = Point3(1, 0.5f, 4) * 1.2f;
    Point3 lookat = Point3(0, 0, 0);

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 30;

    return new PerspectiveCamera(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
}

static int32 index = Sample::Register("normal-mapping", NormalMappingTest);

} // namespace bulbit