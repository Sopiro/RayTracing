#include "spt/spt.h"

namespace spt
{

void Sponza(Scene& scene)
{
    // Transform transform{ zero_vec3, Quat(DegToRad(90.0), y_axis), Vec3(0.01) };
    // Ref<Model> sponza = CreateSharedRef<Model>("res/sponza2/sponza.obj", transform);

    Transform transform{ zero_vec3, Quat(DegToRad(90.0), y_axis), Vec3(1.0) };
    Ref<Model> sponza = CreateSharedRef<Model>("res/sponza/Sponza.gltf", transform);

    scene.Add(sponza);

    auto light = CreateSharedRef<DiffuseLight>(Spectrum(1.0));
    // auto mat = CreateSharedRef<Dielectric>(1.5);

    Float cx = 8.0;
    Float cy = 4.0;
    Float cz = 8.0;

    Float sx = 10.0;
    Float sy = 10.0;
    Float sz = 20.0;

    Float xm = -sx / 2.0;
    Float ym = 0.0;
    Float zm = -sz / 2.0;

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
    //     auto light2 = CreateSharedRef<DiffuseLight>(Spectrum(20.0));
    //     auto sphere = CreateSharedRef<Sphere>(Vec3(0.0, 1.5, 0.0), 0.4, light2);
    //     scene.Add(sphere);
    //     scene.AddLight(sphere);
    // }

    scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/quarry_04_puresky_1k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/solitude_night_4k/solitude_night_4k.hdr"));

    Spectrum sky_color(147 / 255.0, 209 / 255.0, 255 / 255.0);
    scene.AddLight(CreateSharedRef<DirectionalLight>(Normalize(-Vec3(-3.0, 15.0, -3.0)), Vec3(15.0), 0.02));
}

} // namespace spt
