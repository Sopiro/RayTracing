#include "spt/spt.h"

namespace spt
{

void CornellBoxOriginal(Scene& scene)
{
    // Materials
    auto red = CreateSharedRef<Lambertian>(Spectrum(.65, .05, .05));
    auto green = CreateSharedRef<Lambertian>(Spectrum(.12, .45, .15));
    auto blue = CreateSharedRef<Lambertian>(Spectrum(.22, .23, .75));
    auto white = CreateSharedRef<Lambertian>(Spectrum(.73, .73, .73));
    auto wakgood_texture = ImageTexture::Create("res/wakdu.jpg");
    auto wakgood_mat = CreateSharedRef<Lambertian>(wakgood_texture);
    auto light = CreateSharedRef<DiffuseLight>(Spectrum(17.0, 12.0, 4.0));
    // light->two_sided = true;

    // Cornell box
    {
        // front
        auto tf = Transform{ Vec3(0.5, 0.5, -1.0), identity, Vec3(1.0) };
        scene.Add(CreateRectXY(tf, wakgood_mat));

        // left
        tf = Transform{ Vec3(0.0, 0.5, -0.5), identity, Vec3(1.0) };
        scene.Add(CreateRectYZ(tf, red));

        // right
        tf = Transform{ Vec3(1.0, 0.5, -0.5), Quat(pi, y_axis), Vec3(1.0) };
        scene.Add(CreateRectYZ(tf, green));

        // bottom
        tf = Transform{ Vec3(0.5, 0, -0.5), identity, Vec3(1.0) };
        scene.Add(CreateRectXZ(tf, white));

        // top
        tf = Transform{ Vec3(0.5, 1.0, -0.5), Quat(pi, x_axis), Vec3(1.0) };
        scene.Add(CreateRectXZ(tf, white));
    }

    // Left block
    {
        Float hx = 0.14;
        Float hy = 0.28;
        Float hz = 0.14;

        auto tf = Transform{ 0.33, hy, -0.66, Quat(DegToRad(18.0), y_axis), Vec3(hx * 2.0, hy * 2.0, hz * 2.0) };
        // auto box = Box(tf, white);
        auto box = CreateBox(tf, white);

        scene.Add(box);
    }

    // Right block
    {
        Float hx = 0.14;
        Float hy = 0.14;
        Float hz = 0.14;

        auto tf = Transform{ 0.66, hy, -0.33, Quat(DegToRad(-18.0), y_axis), Vec3(hx * 2.0, hy * 2.0, hz * 2.0) };
        auto box = CreateBox(tf, white);

        scene.Add(box);
    }

    // Lights
    {
        auto tf = Transform{ 0.5, 0.998, -0.5, Quat(pi, x_axis), Vec3(0.25) };
        auto l = CreateRectXZ(tf, light);

        scene.AddLight(l);
    }

    // scene.Rebuild();
    // std::cout << "Lights: " << scene.GetLights().GetCount() << std::endl;
}

} // namespace spt