#include "raytracer/raytracer.h"

namespace spt
{

void Sponza(Scene& scene)
{
    // Transform transform{ Vec3{ 0.0, 0.0, 0.0 }, Quat{ DegToRad(90.0), Vec3{ 0.0, 1.0, 0.0 } }, Vec3{ 0.01 } };
    // std::shared_ptr<Model> sponza = std::make_shared<Model>("res/sponza2/sponza.obj", transform);

    Transform transform{ Vec3{ 0.0, 0.0, 0.0 }, Quat{ DegToRad(90.0), Vec3{ 0.0, 1.0, 0.0 } }, Vec3{ 1.0 } };
    std::shared_ptr<Model> sponza = std::make_shared<Model>("res/sponza/Sponza.gltf", transform);

    scene.Add(sponza);

    auto light = std::make_shared<DiffuseLight>(Color{ 1.0 });
    // auto mat = std::make_shared<Dielectric>(1.5);

    double cx = 10.0;
    double cy = 10.0;
    double cz = 10.0;
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

    {
        Transform tf1(Vec3{ 0.0, 10.0, 0.0 }, Quat{ identity }, Vec3{ 1.0 });
        Vertex t0{ tf1 * Vec3{ -0.5, 0.0, 0.5 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 0.0, 0.0 } };
        Vertex t1{ tf1 * Vec3{ 0.5, 0.0, 0.5 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 1.0, 0.0 } };
        Vertex t2{ tf1 * Vec3{ 0.5, 0.0, -0.5 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 1.0, 1.0 } };
        Vertex t3{ tf1 * Vec3{ -0.5, 0.0, -0.5 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 0.0, 1.0 } };

        auto l1 = std::make_shared<Triangle>(t0, t2, t1, light);
        auto l2 = std::make_shared<Triangle>(t0, t3, t2, light);

        // scene.Add(l1);
        // scene.Add(l2);
        // scene.AddLight(l1);
        // scene.AddLight(l2);
    }

    // Color sky_color = Color{ 0.9, 0.8, 0.8 } * 8.0;
    Color sky_color = Color{ 0.7, 0.8, 1.0 } * 10.0;
    // Color sky_color{ 0.0, 0.0, 0.0 };

    scene.SetSkyColor(sky_color);

    // auto sphere = std::make_shared<Sphere>(Vec3(0.0, 10.0, 0.0), 1.0, nullptr);
    // scene.AddLight(sphere);

    // auto light2 = std::make_shared<DiffuseLight>(Color{ 20.0 });
    // auto sphere = std::make_shared<Sphere>(Vec3(0.0, 1.5, 0.0), 0.4, light2);
    // scene.Add(sphere);
    // scene.AddLight(sphere);
}

} // namespace spt
