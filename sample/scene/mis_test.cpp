#include "spt/spt.h"

namespace spt
{

void MISTest(Scene& scene)
{
    {
        auto floor_mat = RandomMicrofacetMaterial();
        floor_mat->basecolor = ConstantColor::Create(Spectrum(0.4));
        floor_mat->roughness = ConstantColor::Create(Spectrum(1.0));
        floor_mat->metallic = ConstantColor::Create(Spectrum(0.0));

        auto floor = CreateSharedRef<Model>("res/veach_mi/floor.obj", Transform{ identity });
        floor->GetMeshes()[0]->SetMaterial(floor_mat);
        scene.Add(floor);
    }

    // plates
    {
        auto m1 = RandomMicrofacetMaterial();
        m1->basecolor = ConstantColor::Create(Spectrum(0.07, 0.09, 0.13));
        m1->metallic = ConstantColor::Create(Spectrum(1.0));
        m1->roughness = ConstantColor::Create(Spectrum(std::sqrt(0.005)));

        auto m2 = RandomMicrofacetMaterial();
        m2->basecolor = ConstantColor::Create(Spectrum(0.07, 0.09, 0.13));
        m2->metallic = ConstantColor::Create(Spectrum(1.0));
        m2->roughness = ConstantColor::Create(Spectrum(std::sqrt(0.02)));

        auto m3 = RandomMicrofacetMaterial();
        m3->basecolor = ConstantColor::Create(Spectrum(0.07, 0.09, 0.13));
        m3->metallic = ConstantColor::Create(Spectrum(1.0));
        m3->roughness = ConstantColor::Create(Spectrum(std::sqrt(0.05)));

        auto m4 = RandomMicrofacetMaterial();
        m4->basecolor = ConstantColor::Create(Spectrum(0.07, 0.09, 0.13));
        m4->metallic = ConstantColor::Create(Spectrum(1.0));
        m4->roughness = ConstantColor::Create(Spectrum(std::sqrt(0.1)));

        auto p1 = CreateSharedRef<Model>("res/veach_mi/plate1.obj", Transform{ identity });
        p1->GetMeshes()[0]->SetMaterial(m1);
        scene.Add(p1);
        auto p2 = CreateSharedRef<Model>("res/veach_mi/plate2.obj", Transform{ identity });
        p2->GetMeshes()[0]->SetMaterial(m2);
        scene.Add(p2);
        auto p3 = CreateSharedRef<Model>("res/veach_mi/plate3.obj", Transform{ identity });
        p3->GetMeshes()[0]->SetMaterial(m3);
        scene.Add(p3);
        auto p4 = CreateSharedRef<Model>("res/veach_mi/plate4.obj", Transform{ identity });
        p4->GetMeshes()[0]->SetMaterial(m4);
        scene.Add(p4);
    }

    // Lights
    {
        auto light1 = CreateSharedRef<DiffuseLight>(Spectrum(800));
        auto light3 = CreateSharedRef<DiffuseLight>(Spectrum(901.803));
        auto light2 = CreateSharedRef<DiffuseLight>(Spectrum(100));
        auto light4 = CreateSharedRef<DiffuseLight>(Spectrum(11.1111));
        auto light5 = CreateSharedRef<DiffuseLight>(Spectrum(1.23457));

        auto l1 = CreateSharedRef<Sphere>(Vec3(10, 10, 4), 0.5, light1);
        auto l3 = CreateSharedRef<Sphere>(Vec3(-3.75, 0, 0), 0.03333, light3);
        auto l2 = CreateSharedRef<Sphere>(Vec3(-1.25, 0, 0), 0.1, light2);
        auto l4 = CreateSharedRef<Sphere>(Vec3(1.25, 0, 0), 0.3, light4);
        auto l5 = CreateSharedRef<Sphere>(Vec3(3.75, 0, 0), 0.9, light5);

        scene.AddLight(l1);
        scene.AddLight(l2);
        scene.AddLight(l3);
        scene.AddLight(l4);
        scene.AddLight(l5);
    }
}

void MISTestWak(Scene& scene)
{
    {
        auto floor = RandomMicrofacetMaterial();
        floor->basecolor = ImageTexture::Create("res/wakdu.jpg");
        floor->roughness = ConstantColor::Create(Spectrum(1.0));
        floor->metallic = ConstantColor::Create(Spectrum(0.0));

        Float s = 20.0;
        auto tf = Transform{ Vec3(0.0, -4.0, -4.0), identity, Vec3(s, 1.0, s) };
        scene.Add(CreateRectXZ(tf, floor));

        tf = Transform{ Vec3(0.0, -4.0, -4.0), identity, Vec3(s, s, 1.0) };
        scene.Add(CreateRectXY(tf, floor));
    }

    // plates
    {
        auto m1 = RandomMicrofacetMaterial();
        m1->basecolor = ConstantColor::Create(Spectrum(0.07, 0.09, 0.13));
        m1->metallic = ConstantColor::Create(Spectrum(1.0));
        m1->roughness = ConstantColor::Create(Spectrum(std::sqrt(0.005)));

        auto m2 = RandomMicrofacetMaterial();
        m2->basecolor = ConstantColor::Create(Spectrum(0.07, 0.09, 0.13));
        m2->metallic = ConstantColor::Create(Spectrum(1.0));
        m2->roughness = ConstantColor::Create(Spectrum(std::sqrt(0.02)));

        auto m3 = RandomMicrofacetMaterial();
        m3->basecolor = ConstantColor::Create(Spectrum(0.07, 0.09, 0.13));
        m3->metallic = ConstantColor::Create(Spectrum(1.0));
        m3->roughness = ConstantColor::Create(Spectrum(std::sqrt(0.05)));

        auto m4 = RandomMicrofacetMaterial();
        m4->basecolor = ConstantColor::Create(Spectrum(0.07, 0.09, 0.13));
        m4->metallic = ConstantColor::Create(Spectrum(1.0));
        m4->roughness = ConstantColor::Create(Spectrum(std::sqrt(0.1)));

        auto p1 = CreateSharedRef<Model>("res/veach_mi/plate1.obj", Transform{ identity });
        p1->GetMeshes()[0]->SetMaterial(m1);
        scene.Add(p1);
        auto p2 = CreateSharedRef<Model>("res/veach_mi/plate2.obj", Transform{ identity });
        p2->GetMeshes()[0]->SetMaterial(m2);
        scene.Add(p2);
        auto p3 = CreateSharedRef<Model>("res/veach_mi/plate3.obj", Transform{ identity });
        p3->GetMeshes()[0]->SetMaterial(m3);
        scene.Add(p3);
        auto p4 = CreateSharedRef<Model>("res/veach_mi/plate4.obj", Transform{ identity });
        p4->GetMeshes()[0]->SetMaterial(m4);
        scene.Add(p4);
    }

    // Lights
    {
        auto light1 = CreateSharedRef<DiffuseLight>(Spectrum(800));
        auto light3 = CreateSharedRef<DiffuseLight>(Spectrum(901.803, 0, 0));
        auto light2 = CreateSharedRef<DiffuseLight>(Spectrum(100, 100 / 5, 0));
        auto light4 = CreateSharedRef<DiffuseLight>(Spectrum(11.1111, 11.1111, 0));
        auto light5 = CreateSharedRef<DiffuseLight>(Spectrum(0, 1.23457, 0));

        auto l1 = CreateSharedRef<Sphere>(Vec3(10, 10, 4), 0.5, light1);
        auto l3 = CreateSharedRef<Sphere>(Vec3(-3.75, 0, 0), 0.03333, light3);
        auto l2 = CreateSharedRef<Sphere>(Vec3(-1.25, 0, 0), 0.1, light2);
        auto l4 = CreateSharedRef<Sphere>(Vec3(1.25, 0, 0), 0.3, light4);
        auto l5 = CreateSharedRef<Sphere>(Vec3(3.75, 0, 0), 0.9, light5);

        // scene.AddLight(l1);
        scene.AddLight(l2);
        scene.AddLight(l3);
        scene.AddLight(l4);
        scene.AddLight(l5);
    }
}

} // namespace spt