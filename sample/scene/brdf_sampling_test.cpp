#include "spt/pathtracer.h"

namespace spt
{

void BRDFSamplingTest(Scene& scene)
{
    // Materials
    auto red = CreateSharedRef<Lambertian>(Color{ .65, .05, .05 });
    auto green = CreateSharedRef<Lambertian>(Color{ .12, .45, .15 });
    auto blue = CreateSharedRef<Lambertian>(Color{ .22, .23, .75 });
    auto white = CreateSharedRef<Lambertian>(Color{ .73, .73, .73 });
    auto wakgood_texture = ImageTexture::Create("res/wakdu.jpg");
    auto wakgood_mat = CreateSharedRef<Lambertian>(wakgood_texture);
    auto light = CreateSharedRef<DiffuseLight>(Color{ 15.0 });

    // Cornell box
    {
        // front
        auto tf = Transform{ Vec3{ 0.5, 0.5, -1.0 }, Quat{ identity }, Vec3{ 1.0 } };
        scene.Add(CreateRectXY(tf, wakgood_mat));

        // left
        tf = Transform{ Vec3{ 0.0, 0.5, -0.5 }, Quat{ identity }, Vec3{ 1.0 } };
        scene.Add(CreateRectYZ(tf, red));

        // right
        tf = Transform{ Vec3{ 1.0, 0.5, -0.5 }, Quat{ pi, y_axis }, Vec3{ 1.0 } };
        scene.Add(CreateRectYZ(tf, green));

        // bottom
        tf = Transform{ Vec3{ 0.5, 0, -0.5 }, Quat{ identity }, Vec3{ 1.0 } };
        scene.Add(CreateRectXZ(tf, white));

        // top
        tf = Transform{ Vec3{ 0.5, 1.0, -0.5 }, Quat{ pi, x_axis }, Vec3{ 1.0 } };
        scene.Add(CreateRectXZ(tf, white));
    }

    // Lights
    {
        auto tf = Transform{ 0.5, 0.999, -0.5, Quat{ pi, x_axis }, Vec3{ 0.25 } };
        auto l = CreateRectXZ(tf, light);

        scene.Add(l);
        scene.AddAreaLight(l);
    }

    // Center sphere
    {
        auto mat = RandomPBRMaterial();
        mat->basecolor_map = SolidColor::Create(Vec3{ 1.0 });
        mat->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        mat->roughness_map = SolidColor::Create(Vec3{ 0.2 });

        f64 r = 0.25;
        auto sphere = CreateSharedRef<Sphere>(Vec3(0.5, r, -0.5), r, mat);

        scene.Add(sphere);
        // scene.AddLight(sphere);
    }

    // scene.Rebuild();
    // std::cout << scene.GetLights().GetCount() << std::endl;
}

} // namespace spt