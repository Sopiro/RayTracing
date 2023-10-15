#include "spt/spt.h"

namespace spt
{

// https://developer.nvidia.com/orca/amazon-lumberyard-bistro
void BistroScene(Scene& scene)
{
    Transform tf{ zero_vec3, identity, Vec3(1.0) };
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
}

} // namespace spt