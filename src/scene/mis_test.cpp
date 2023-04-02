#include "raytracer/raytracer.h"

namespace spt
{

void MISTest(Scene& scene)
{
    {
        auto wall = RandomPBRMaterial();
        wall->basecolor_map = SolidColor::Create(Vec3{ 0.73 });
        wall->roughness_map = SolidColor::Create(Vec3{ 1.0 });
        wall->metallic_map = SolidColor::Create(Vec3{ 0.0 });

        auto tf = Transform{ Vec3{ 0.0, 0.0, 0.0 }, Quat{ identity }, Vec3{ 3.0, 1.0, 3.0 } };
        scene.Add(RectXZ(tf, wall));

        tf = Transform{ Vec3{ 0.0, 0.0, -1.0 }, Quat{ identity }, Vec3{ 3.0, 3.0, 1.0 } };
        scene.Add(RectXY(tf, wall));
    }

    double y = 0.05;
    double z = 0.0;

    // plates
    {
        auto m1 = RandomPBRMaterial();
        m1->basecolor_map = SolidColor::Create(Vec3{ 1.0 });
        m1->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        m1->roughness_map = SolidColor::Create(Vec3{ 0.25 });

        auto m2 = RandomPBRMaterial();
        m2->basecolor_map = SolidColor::Create(Vec3{ 1.0 });
        m2->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        m2->roughness_map = SolidColor::Create(Vec3{ 0.16 });

        auto m3 = RandomPBRMaterial();
        m3->basecolor_map = SolidColor::Create(Vec3{ 1.0 });
        m3->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        m3->roughness_map = SolidColor::Create(Vec3{ 0.12 });

        auto m4 = RandomPBRMaterial();
        m4->basecolor_map = SolidColor::Create(Vec3{ 1.0 });
        m4->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        m4->roughness_map = SolidColor::Create(Vec3{ 0.08 });

        double h = 0.2;
        double dh = 0.02;
        double w = 1.0;

        double a1 = DegToRad(15.0);
        double a2 = DegToRad(25.0);
        double a3 = DegToRad(35.0);
        double a4 = DegToRad(45.0);

        auto tf = Transform{ Vec3{ 0.0, y, z }, Quat{ a1, x_axis }, Vec3{ w, h, h - dh } };
        scene.Add(RectXZ(tf, m1));

        y += (sin(a1) * h + sin(a2) * h) / 2.0;
        z -= (cos(a1) * h + cos(a2) * h) / 2.0;

        tf = Transform{ Vec3{ 0.0, y, z }, Quat{ a2, x_axis }, Vec3{ w, h, h - dh } };
        scene.Add(RectXZ(tf, m2));

        y += (sin(a2) * h + sin(a3) * h) / 2.0;
        z -= (cos(a2) * h + cos(a3) * h) / 2.0;

        tf = Transform{ Vec3{ 0.0, y, z }, Quat{ a3, x_axis }, Vec3{ w, h, h - dh } };
        scene.Add(RectXZ(tf, m3));

        y += (sin(a3) * h + sin(a4) * h) / 2.0;
        z -= (cos(a3) * h + cos(a4) * h) / 2.0;

        tf = Transform{ Vec3{ 0.0, y, z }, Quat{ a4, x_axis }, Vec3{ w, h, h - dh } };
        scene.Add(RectXZ(tf, m4));
    }

    std::cout << y << ", " << z << std::endl;

    // Lights
    {
        auto light = std::make_shared<DiffuseLight>(Color{ 20.0 });

        double lh = y;

        auto l1 = std::make_shared<Sphere>(Vec3{ -0.125 * 3.0, y + lh, z }, 0.008, light);
        auto l2 = std::make_shared<Sphere>(Vec3{ -0.125, y + lh, z }, 0.016, light);
        auto l3 = std::make_shared<Sphere>(Vec3{ 0.125, y + lh, z }, 0.032, light);
        auto l4 = std::make_shared<Sphere>(Vec3{ 0.125 * 3.0, y + lh, z }, 0.064, light);

        scene.Add(l1);
        scene.AddLight(l1);
        scene.Add(l2);
        scene.AddLight(l2);
        scene.Add(l3);
        scene.AddLight(l3);
        scene.Add(l4);
        scene.AddLight(l4);
    }

    scene.SetEnvironmentMap(SolidColor::Create(Vec3{ 0.01 }));
}

} // namespace spt