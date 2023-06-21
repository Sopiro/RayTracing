#include "spt/pathtracer.h"

namespace spt
{

void Sponza(Scene& scene)
{
    // Transform transform{ zero_vec3, Quat{ DegToRad(90.0), y_axis }, Vec3{ 0.01 } };
    // Ref<Model> sponza = CreateSharedRef<Model>("res/sponza2/sponza.obj", transform);

    Transform transform{ zero_vec3, Quat{ DegToRad(90.0), y_axis }, Vec3{ 1.0 } };
    Ref<Model> sponza = CreateSharedRef<Model>("res/sponza/Sponza.gltf", transform);

    scene.Add(sponza);

    auto light = CreateSharedRef<DiffuseLight>(Color{ 1.0 });
    // auto mat = CreateSharedRef<Dielectric>(1.5);

    float64 cx = 8.0;
    float64 cy = 4.0;
    float64 cz = 8.0;

    float64 sx = 10.0;
    float64 sy = 10.0;
    float64 sz = 20.0;

    float64 xm = -sx / 2.0;
    float64 ym = 0.0;
    float64 zm = -sz / 2.0;

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
    //     auto light2 = CreateSharedRef<DiffuseLight>(Color{ 20.0 });
    //     auto sphere = CreateSharedRef<Sphere>(Vec3(0.0, 1.5, 0.0), 0.4, light2);
    //     scene.Add(sphere);
    //     scene.AddLight(sphere);
    // }

    scene.SetEnvironmentMap(ImageTexture::Create("res/sunflowers/sunflowers_puresky_4k.hdr", false, true));
    // scene.SetEnvironmentMap(SolidColor::Create(Color{ 1.0 }));

    scene.SetDirectionalLight(CreateSharedRef<DirectionalLight>(-Vec3{ -3.0, 15.0, -3.0 }.Normalized(), Vec3{ 10.0 }));
}

} // namespace spt
