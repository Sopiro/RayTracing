#include "spt/pathtracer.h"

namespace spt
{

// https://casual-effects.com/data/
void BreakfastRoom(Scene& scene)
{
    Transform tf{ zero_vec3, Quat{ DegToRad(0.0), y_axis }, Vec3{ 1.0 } };
    Ref<Model> m = CreateSharedRef<Model>("res/breakfast_room/breakfast_room.obj", tf);

    scene.Add(m);

    scene.SetEnvironmentMap(SolidColor::Create(Color{ 5.0 }));
    scene.SetDirectionalLight(CreateSharedRef<DirectionalLight>(-Vec3{ 15.0, 5.0, 5.0 }.Normalized(), Vec3{ 20.0 }, 0.01));
}

} // namespace spt