#include "raytracer/raytracer.h"

namespace spt
{

void Sponza(Scene& scene)
{
    Transform transform{ zero_vec3, Quat{ DegToRad(90.0), y_axis }, Vec3{ 1.0 } };
    std::shared_ptr<Model> sponza = std::make_shared<Model>("res/sponza/Sponza.gltf", transform);

    scene.Add(sponza);

    auto light = std::make_shared<DiffuseLight>(Color{ 1.0 });
    // auto mat = std::make_shared<Dielectric>(1.5);

    double cx = 10.0;
    double cy = 10.0;
    double cz = 20.0;

    double sx = 10.0;
    double sy = 10.0;
    double sz = 20.0;

    double xm = -sx / 2.0;
    double ym = 0.0;
    double zm = -sz / 2.0;

    // for (int32 z = 0; z < cz; ++z)
    // {
    //     for (int32 y = 0; y < cy; ++y)
    //     {
    //         for (int32 x = 0; x < cx; ++x)
    //         {
    //             auto sphere = std::make_shared<Sphere>(Vec3(x / cx * sx + xm, y / cy * sy + ym, z / cz * sz + zm), 0.1, light);
    //             scene.Add(sphere);
    //             scene.AddLight(sphere);
    //         }
    //     }
    // }

    // {
    //     auto light2 = std::make_shared<DiffuseLight>(Color{ 20.0 });
    //     auto sphere = std::make_shared<Sphere>(Vec3(0.0, 1.5, 0.0), 0.4, light2);
    //     scene.Add(sphere);
    //     scene.AddLight(sphere);
    // }

    scene.SetDirectionalLight(std::make_shared<DirectionalLight>(-Vec3{ -3.0, 15.0, 0.0 }.Normalized(), Vec3{ 10.0 }));
    scene.SetEnvironmentMap(ImageTexture::Create("res/sunflowers/sunflowers_puresky_4k.hdr", false, true));
    // scene.SetEnvironmentMap(SolidColor::Create(Color{ 1.0 }));
}

} // namespace spt
