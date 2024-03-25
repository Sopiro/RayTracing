#include "../samples.h"
#include "bulbit/diffuse_light.h"
#include "bulbit/lambertian.h"
#include "bulbit/perspective_camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

// https://casual-effects.com/data/
Camera* BreakfastRoom(Scene& scene)
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

    Float aspect_ratio = 16. / 9.;
    // Float aspect_ratio = 3. / 2.;
    // Float aspect_ratio = 4. / 3.;
    // Float aspect_ratio = 1.;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0, 2.2f, 4.5f };
    Point3 lookat{ 0, 1.5f, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 71;

    return new PerspectiveCamera(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("breakfast-room", BreakfastRoom);

} // namespace bulbit