#include "spt/spt.h"

namespace spt
{

void NormalMapping(Scene& scene)
{
    // Transform transform{ zero_vec3, Quat(DegToRad(0.0), y_axis), Vec3(0.01) };
    // Ref<Model> model = CreateSharedRef<Model>("res/pbr_kabuto_samurai_helmet/scene.gltf", transform);

    Transform transform{ zero_vec3, Quat(DegToRad(0.0), y_axis), Vec3(1.0) };
    Ref<Model> model = CreateSharedRef<Model>("res/DamagedHelmet/DamagedHelmet.gltf", transform);

    scene.Add(model);

    // scene.SetEnvironmentMap(ImageTexture::Create("res/sunflowers/sunflowers_puresky_4k.hdr", false, true));
    scene.SetEnvironmentMap(ImageTexture::Create("res/solitude_night_4k/solitude_night_4k.hdr", false, true));

    // scene.AddLight(CreateSharedRef<DirectionalLight>(Normalize(-Vec3(-1, 10, 0)), Vec3(1.0), 0.01));
    // scene.SetEnvironmentMap(SolidColor::Create(zero_vec3));
}

} // namespace spt