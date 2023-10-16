#include "spt/spt.h"

namespace spt
{

// https://casual-effects.com/data/
void BreakfastRoom(Scene& scene)
{
    Transform tf{ zero_vec3, Quat(DegToRad(0.0f), y_axis), Vec3(1.0f) };
    Ref<Model> m = CreateSharedRef<Model>("res/breakfast_room/breakfast_room.obj", tf);

    scene.Add(m);

    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/kloppenheim_07_puresky_1k.hdr"));
    scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/quarry_04_puresky_1k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr"));

    scene.AddLight(CreateSharedRef<DirectionalLight>(Normalize(-Vec3(15.0f, 5.0f, 5.0f)), Vec3(8.0f), 0.02f));

    // auto light = CreateSharedRef<DiffuseLight>(Spectrum(5000.0));
    // tf = Transform{ 30, 10, 15, Quat(pi, z_axis), Vec3(1.0, 1.5, 5.0) };
    // auto l = CreateRectYZ(tf, light);

    // scene.AddLight(l);
}

} // namespace spt