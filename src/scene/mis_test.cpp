#include "raytracer/raytracer.h"

namespace spt
{

void MISTest1(Scene& scene)
{
    {
        auto floor = RandomPBRMaterial();
        floor->basecolor_map = SolidColor::Create(Vec3{ 0.73 });
        floor->roughness_map = SolidColor::Create(Vec3{ 1.0 });
        floor->metallic_map = SolidColor::Create(Vec3{ 0.0 });

        auto tf = Transform{ Vec3{ 0.0, 0.0, 0.0 }, Quat{ identity }, Vec3{ 3.0, 1.0, 3.0 } };
        scene.Add(RectXZ(tf, floor));

        tf = Transform{ Vec3{ 0.0, 0.0, -1.0 }, Quat{ identity }, Vec3{ 3.0, 3.0, 1.0 } };
        scene.Add(RectXY(tf, floor));
    }

    double y = 0.05;
    double z = 0.0;

    // plates
    {
        auto m1 = RandomPBRMaterial();
        m1->basecolor_map = SolidColor::Create(Vec3{ 0.07, 0.09, 0.13 });
        m1->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        m1->roughness_map = SolidColor::Create(Vec3{ 0.1 });

        auto m2 = RandomPBRMaterial();
        m2->basecolor_map = SolidColor::Create(Vec3{ 0.07, 0.09, 0.13 });
        m2->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        m2->roughness_map = SolidColor::Create(Vec3{ 0.05 });

        auto m3 = RandomPBRMaterial();
        m3->basecolor_map = SolidColor::Create(Vec3{ 0.07, 0.09, 0.13 });
        m3->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        m3->roughness_map = SolidColor::Create(Vec3{ 0.02 });

        auto m4 = RandomPBRMaterial();
        m4->basecolor_map = SolidColor::Create(Vec3{ 0.07, 0.09, 0.13 });
        m4->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        m4->roughness_map = SolidColor::Create(Vec3{ 0.005 });

        double h = 0.2;
        double dh = 0.025;
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

    // std::cout << y << ", " << z << std::endl;

    // Lights
    {
        auto light1 = std::make_shared<DiffuseLight>(Color{ 3000.0 });
        auto light2 = std::make_shared<DiffuseLight>(Color{ 300.0 });
        auto light3 = std::make_shared<DiffuseLight>(Color{ 30.0 });
        auto light4 = std::make_shared<DiffuseLight>(Color{ 3.0 });

        double lh = y;
        double xg = 0.16;

        double r = 0.1;
        auto l1 = std::make_shared<Sphere>(Vec3{ -xg * 3.0, y + lh, z }, r / 27.0, light1);
        auto l2 = std::make_shared<Sphere>(Vec3{ -xg, y + lh, z }, r / 9.0, light2);
        auto l3 = std::make_shared<Sphere>(Vec3{ xg, y + lh, z }, r / 3.0, light3);
        auto l4 = std::make_shared<Sphere>(Vec3{ xg * 3.0, y + lh, z }, r, light4);

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

void MISTest2(Scene& scene)
{
    {
        auto floor_mat = RandomPBRMaterial();
        floor_mat->basecolor_map = SolidColor::Create(Vec3{ 0.4 });
        floor_mat->roughness_map = SolidColor::Create(Vec3{ 1.0 });
        floor_mat->metallic_map = SolidColor::Create(Vec3{ 0.0 });

        auto floor = std::make_shared<Model>("res/veach_mi/floor.obj", Transform{ identity });
        floor->GetMeshes()[0]->SetMaterial(floor_mat);
        scene.Add(floor);
    }

    // plates
    {
        auto m1 = RandomPBRMaterial();
        m1->basecolor_map = SolidColor::Create(Vec3{ 0.07, 0.09, 0.13 });
        m1->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        m1->roughness_map = SolidColor::Create(Vec3{ 0.005 });

        auto m2 = RandomPBRMaterial();
        m2->basecolor_map = SolidColor::Create(Vec3{ 0.07, 0.09, 0.13 });
        m2->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        m2->roughness_map = SolidColor::Create(Vec3{ 0.02 });

        auto m3 = RandomPBRMaterial();
        m3->basecolor_map = SolidColor::Create(Vec3{ 0.07, 0.09, 0.13 });
        m3->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        m3->roughness_map = SolidColor::Create(Vec3{ 0.05 });

        auto m4 = RandomPBRMaterial();
        m4->basecolor_map = SolidColor::Create(Vec3{ 0.07, 0.09, 0.13 });
        m4->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        m4->roughness_map = SolidColor::Create(Vec3{ 0.1 });

        auto p1 = std::make_shared<Model>("res/veach_mi/plate1.obj", Transform{ identity });
        p1->GetMeshes()[0]->SetMaterial(m1);
        scene.Add(p1);
        auto p2 = std::make_shared<Model>("res/veach_mi/plate2.obj", Transform{ identity });
        p2->GetMeshes()[0]->SetMaterial(m2);
        scene.Add(p2);
        auto p3 = std::make_shared<Model>("res/veach_mi/plate3.obj", Transform{ identity });
        p3->GetMeshes()[0]->SetMaterial(m3);
        scene.Add(p3);
        auto p4 = std::make_shared<Model>("res/veach_mi/plate4.obj", Transform{ identity });
        p4->GetMeshes()[0]->SetMaterial(m4);
        scene.Add(p4);
    }

    // Lights
    {
        auto light1 = std::make_shared<DiffuseLight>(Color{ 800 });
        auto light3 = std::make_shared<DiffuseLight>(Color{ 901.803 });
        auto light2 = std::make_shared<DiffuseLight>(Color{ 100 });
        auto light4 = std::make_shared<DiffuseLight>(Color{ 11.1111 });
        auto light5 = std::make_shared<DiffuseLight>(Color{ 1.23457 });

        auto l1 = std::make_shared<Sphere>(Vec3{ 10, 10, 4 }, 0.5, light1);
        auto l3 = std::make_shared<Sphere>(Vec3{ -3.75, 0, 0 }, 0.03333, light3);
        auto l2 = std::make_shared<Sphere>(Vec3{ -1.25, 0, 0 }, 0.1, light2);
        auto l4 = std::make_shared<Sphere>(Vec3{ 1.25, 0, 0 }, 0.3, light4);
        auto l5 = std::make_shared<Sphere>(Vec3{ 3.75, 0, 0 }, 0.9, light5);

        scene.Add(l1);
        scene.AddLight(l1);
        scene.Add(l2);
        scene.AddLight(l2);
        scene.Add(l3);
        scene.AddLight(l3);
        scene.Add(l4);
        scene.AddLight(l4);
        scene.Add(l5);
        scene.AddLight(l5);
    }

    scene.SetEnvironmentMap(SolidColor::Create(Vec3{ 0.0 }));
}

void MISTestWak(Scene& scene)
{
    {
        auto floor = RandomPBRMaterial();
        floor->basecolor_map = ImageTexture::Create("res/wakdu.jpg");
        floor->roughness_map = SolidColor::Create(Vec3{ 1.0 });
        floor->metallic_map = SolidColor::Create(Vec3{ 0.0 });

        double s = 20.0;
        auto tf = Transform{ Vec3{ 0.0, -4.0, -4.0 }, Quat{ identity }, Vec3{ s, 1.0, s } };
        scene.Add(RectXZ(tf, floor));

        tf = Transform{ Vec3{ 0.0, -4.0, -4.0 }, Quat{ identity }, Vec3{ s, s, 1.0 } };
        scene.Add(RectXY(tf, floor));
    }

    // plates
    {
        auto m1 = RandomPBRMaterial();
        m1->basecolor_map = SolidColor::Create(Vec3{ 0.07, 0.09, 0.13 });
        m1->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        m1->roughness_map = SolidColor::Create(Vec3{ 0.005 });

        auto m2 = RandomPBRMaterial();
        m2->basecolor_map = SolidColor::Create(Vec3{ 0.07, 0.09, 0.13 });
        m2->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        m2->roughness_map = SolidColor::Create(Vec3{ 0.02 });

        auto m3 = RandomPBRMaterial();
        m3->basecolor_map = SolidColor::Create(Vec3{ 0.07, 0.09, 0.13 });
        m3->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        m3->roughness_map = SolidColor::Create(Vec3{ 0.05 });

        auto m4 = RandomPBRMaterial();
        m4->basecolor_map = SolidColor::Create(Vec3{ 0.07, 0.09, 0.13 });
        m4->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        m4->roughness_map = SolidColor::Create(Vec3{ 0.1 });

        auto p1 = std::make_shared<Model>("res/veach_mi/plate1.obj", Transform{ identity });
        p1->GetMeshes()[0]->SetMaterial(m1);
        scene.Add(p1);
        auto p2 = std::make_shared<Model>("res/veach_mi/plate2.obj", Transform{ identity });
        p2->GetMeshes()[0]->SetMaterial(m2);
        scene.Add(p2);
        auto p3 = std::make_shared<Model>("res/veach_mi/plate3.obj", Transform{ identity });
        p3->GetMeshes()[0]->SetMaterial(m3);
        scene.Add(p3);
        auto p4 = std::make_shared<Model>("res/veach_mi/plate4.obj", Transform{ identity });
        p4->GetMeshes()[0]->SetMaterial(m4);
        scene.Add(p4);
    }

    // Lights
    {
        auto light1 = std::make_shared<DiffuseLight>(Color{ 800 });
        auto light3 = std::make_shared<DiffuseLight>(Color{ 901.803, 0, 0 });
        auto light2 = std::make_shared<DiffuseLight>(Color{ 100, 100 / 5, 0 });
        auto light4 = std::make_shared<DiffuseLight>(Color{ 11.1111, 11.1111, 0 });
        auto light5 = std::make_shared<DiffuseLight>(Color{ 0, 1.23457, 0 });

        auto l1 = std::make_shared<Sphere>(Vec3{ 10, 10, 4 }, 0.5, light1);
        auto l3 = std::make_shared<Sphere>(Vec3{ -3.75, 0, 0 }, 0.03333, light3);
        auto l2 = std::make_shared<Sphere>(Vec3{ -1.25, 0, 0 }, 0.1, light2);
        auto l4 = std::make_shared<Sphere>(Vec3{ 1.25, 0, 0 }, 0.3, light4);
        auto l5 = std::make_shared<Sphere>(Vec3{ 3.75, 0, 0 }, 0.9, light5);

        // scene.Add(l1);
        // scene.AddLight(l1);
        scene.Add(l2);
        scene.AddLight(l2);
        scene.Add(l3);
        scene.AddLight(l3);
        scene.Add(l4);
        scene.AddLight(l4);
        scene.Add(l5);
        scene.AddLight(l5);
    }

    scene.SetEnvironmentMap(SolidColor::Create(Vec3{ 0.333 }));
}

} // namespace spt