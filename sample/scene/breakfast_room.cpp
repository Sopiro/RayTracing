#include "spt/spt.h"

namespace spt
{

// https://casual-effects.com/data/
void BreakfastRoom(Scene& scene)
{
    Transform tf{ zero_vec3, Quat(DegToRad(0.0), y_axis), Vec3(1.0) };
    Ref<Model> m = CreateSharedRef<Model>("res/breakfast_room/breakfast_room.obj", tf);

    scene.Add(m);

    scene.AddLight(CreateSharedRef<InfiniteAreaLight>(ConstantColor::Create(Color(7.0))));
    scene.AddLight(CreateSharedRef<DirectionalLight>(Normalize(-Vec3(15.0, 5.0, 5.0)), Vec3(20.0), 0.02));

    // auto light = CreateSharedRef<DiffuseLight>(Color(5000.0));
    // tf = Transform{ 30, 10, 15, Quat(pi, z_axis), Vec3(1.0, 1.5, 5.0) };
    // auto l = CreateRectYZ(tf, light);

    // scene.AddLight(l);
}

} // namespace spt