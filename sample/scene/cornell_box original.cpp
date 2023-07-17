#include "spt/pathtracer.h"

namespace spt
{

void CornellBoxOriginal(Scene& scene)
{
    // Materials
    auto red = CreateSharedRef<Lambertian>(Color{ .65, .05, .05 });
    auto green = CreateSharedRef<Lambertian>(Color{ .12, .45, .15 });
    auto blue = CreateSharedRef<Lambertian>(Color{ .22, .23, .75 });
    auto white = CreateSharedRef<Lambertian>(Color{ .73, .73, .73 });
    auto wakgood_texture = ImageTexture::Create("res/wakdu.jpg");
    auto wakgood_mat = CreateSharedRef<Lambertian>(wakgood_texture);
    auto light = CreateSharedRef<DiffuseLight>(Color{ 17.0, 12.0, 4.0 });
    // light->two_sided = true;

    // Cornell box
    {
        // front
        auto tf = Transform{ Vec3{ 0.5, 0.5, -1.0 }, Quat{ identity }, Vec3{ 1.0 } };
        scene.Add(RectXY(tf, wakgood_mat));

        // left
        tf = Transform{ Vec3{ 0.0, 0.5, -0.5 }, Quat{ identity }, Vec3{ 1.0 } };
        scene.Add(RectYZ(tf, red));

        // right
        tf = Transform{ Vec3{ 1.0, 0.5, -0.5 }, Quat{ pi, y_axis }, Vec3{ 1.0 } };
        scene.Add(RectYZ(tf, green));

        // bottom
        tf = Transform{ Vec3{ 0.5, 0, -0.5 }, Quat{ identity }, Vec3{ 1.0 } };
        scene.Add(RectXZ(tf, white));

        // top
        tf = Transform{ Vec3{ 0.5, 1.0, -0.5 }, Quat{ pi, x_axis }, Vec3{ 1.0 } };
        scene.Add(RectXZ(tf, white));
    }

    // Left block
    {
        float64 hx = 0.14;
        float64 hy = 0.28;
        float64 hz = 0.14;

        auto tf = Transform{ 0.33, hy, -0.66, Quat(DegToRad(18.0), y_axis), Vec3{ hx * 2.0, hy * 2.0, hz * 2.0 } };
        // auto box = Box(tf, white);
        auto box = Box(tf, white);

        scene.Add(box);
    }

    // Right block
    {
        float64 hx = 0.14;
        float64 hy = 0.14;
        float64 hz = 0.14;

        auto tf = Transform{ 0.66, hy, -0.33, Quat(DegToRad(-18.0), y_axis), Vec3{ hx * 2.0, hy * 2.0, hz * 2.0 } };
        auto box = Box(tf, white);

        scene.Add(box);
    }

    // Lights
    {
        auto tf = Transform{ 0.5, 0.998, -0.5, Quat{ pi, x_axis }, Vec3{ 0.25 } };
        auto l = RectXZ(tf, light);

        scene.Add(l);
        scene.AddLight(l);
    }

    // scene.Rebuild();
    // std::cout << "Lights: " << scene.GetLights().GetCount() << std::endl;
}

} // namespace spt