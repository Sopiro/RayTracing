#include "../samples.h"
#include "bulbit/diffuse_light.h"
#include "bulbit/lambertian.h"
#include "bulbit/perspective_camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

Camera* Sponza(Scene& scene)
{
    // Transform transform{ zero_vec3, Quat(DegToRad(90.0f), y_axis), Vec3(0.01f) };
    // Ref<Model> sponza = CreateSharedRef<Model>("res/sponza2/sponza.obj", transform);

    Transform transform{ zero_vec3, Quat(DegToRad(90.0f), y_axis), Vec3(1.0f) };
    Ref<Model> sponza = CreateSharedRef<Model>("res/sponza/Sponza.gltf", transform);

    scene.Add(sponza);

    auto light = CreateSharedRef<DiffuseLight>(Spectrum(1.0f));
    // auto mat = CreateSharedRef<Dielectric>(1.5f);

    Float cx = 8.0f;
    Float cy = 4.0f;
    Float cz = 8.0f;

    Float sx = 10.0f;
    Float sy = 10.0f;
    Float sz = 20.0f;

    Float xm = -sx / 2.0f;
    Float ym = 0.0f;
    Float zm = -sz / 2.0f;

    // for (int32 z = 0; z < cz; ++z)
    // {
    //     for (int32 y = 0; y < cy; ++y)
    //     {
    //         for (int32 x = 0; x < cx; ++x)
    //         {
    //             auto sphere = CreateSharedRef<Sphere>(Vec3(x / cx * sx + xm, y / cy * sy + ym, z / cz * sz + zm), 0.1, light);
    //             scene.Add(sphere);
    //             scene.AddLight(sphere);
    //         }
    //     }
    // }

    // {
    //     auto light2 = CreateSharedRef<DiffuseLight>(Spectrum(20.0f));
    //     auto sphere = CreateSharedRef<Sphere>(Vec3(0.0f, 1.5f, 0.0f), 0.4f, light2);
    //     scene.Add(sphere);
    //     scene.AddLight(sphere);
    // }

    scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/quarry_04_puresky_1k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/solitude_night_4k/solitude_night_4k.hdr"));

    Spectrum sky_color(147 / 255.0f, 209 / 255.0f, 255 / 255.0f);
    scene.AddLight(CreateSharedRef<DirectionalLight>(Normalize(-Vec3(-3.0f, 15.0f, -3.0f)), Vec3(15.0f), 0.02f));

    Float aspect_ratio = 16.0f / 9.0f;

    // Point3 lookfrom{ 0.0f, 2.5f, 4.5f };
    // Point3 lookat{ 0.0f, 1.45f, 0.0f };

    // Point3 lookfrom{ -1.5f, 5.f5, 10.0f };
    // Point3 lookat{ 0.0f, 3.45f, 0.0f };

    // Point3 lookfrom{ 0.0f, 0.5f, 7.0f };
    // Point3 lookat{ 0.0f, 3.0f, 0.0f };

    Point3 lookfrom{ 0, 5, 6 };
    Point3 lookat{ 0, 5, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 71;

    return new PerspectiveCamera(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
}

static int32 index = Sample::Register("sponza", Sponza);

} // namespace bulbit
