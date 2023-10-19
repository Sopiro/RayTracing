#include "bulbit/bulbit.h"

namespace bulbit
{

void NormalMapping(Scene& scene)
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
}

} // namespace bulbit