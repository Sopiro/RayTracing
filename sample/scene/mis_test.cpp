#include "../samples.h"
#include "bulbit/diffuse_light.h"
#include "bulbit/lambertian.h"
#include "bulbit/perspective_camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

std::unique_ptr<Camera> MISTest(Scene& scene)
{
    {
        auto floor_mat = std::make_shared<Microfacet>(ConstantColor::Create(0.4f), ConstantColor::Create(Spectrum(0.0f)),
                                                      ConstantColor::Create(Spectrum(0.0f)));
        auto floor = std::make_shared<Model>("res/veach_mi/floor.obj", Transform{ identity });
        floor->GetMeshes()[0]->SetMaterial(floor_mat);
        scene.Add(floor);
    }

    // plates
    {
        auto m1 = std::make_shared<Microfacet>(ConstantColor::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                               ConstantColor::Create(Spectrum(1.0f)),
                                               ConstantColor::Create(Spectrum(std::sqrt(0.005f))));

        auto m2 = std::make_shared<Microfacet>(ConstantColor::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                               ConstantColor::Create(Spectrum(1.0f)),
                                               ConstantColor::Create(Spectrum(std::sqrt(0.02f))));

        auto m3 = std::make_shared<Microfacet>(ConstantColor::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                               ConstantColor::Create(Spectrum(1.0f)),
                                               ConstantColor::Create(Spectrum(std::sqrt(0.05f))));

        auto m4 =
            std::make_shared<Microfacet>(ConstantColor::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                         ConstantColor::Create(Spectrum(1.0f)), ConstantColor::Create(Spectrum(std::sqrt(0.1f))));

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
        auto light1 = std::make_shared<DiffuseLight>(Spectrum(800.f));
        auto light3 = std::make_shared<DiffuseLight>(Spectrum(901.803f));
        auto light2 = std::make_shared<DiffuseLight>(Spectrum(100.f));
        auto light4 = std::make_shared<DiffuseLight>(Spectrum(11.1111f));
        auto light5 = std::make_shared<DiffuseLight>(Spectrum(1.23457f));

        auto l1 = std::make_shared<Sphere>(Vec3(10, 10, 4), 0.5f, light1);
        auto l3 = std::make_shared<Sphere>(Vec3(-3.75f, 0, 0), 0.03333f, light3);
        auto l2 = std::make_shared<Sphere>(Vec3(-1.25f, 0, 0), 0.1f, light2);
        auto l4 = std::make_shared<Sphere>(Vec3(1.25f, 0, 0), 0.3f, light4);
        auto l5 = std::make_shared<Sphere>(Vec3(3.75f, 0, 0), 0.9f, light5);

        scene.AddLight(l1);
        scene.AddLight(l2);
        scene.AddLight(l3);
        scene.AddLight(l4);
        scene.AddLight(l5);
    }

    Float aspect_ratio = 16. / 9.;
    // Float aspect_ratio = 3. / 2.;
    // Float aspect_ratio = 4. / 3.;
    // Float aspect_ratio = 1.;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0, 2, 15 };
    Point3 lookat{ 0, -2, 2.5f };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 28;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index1 = Sample::Register("mis", MISTest);

std::unique_ptr<Camera> MISTestWak(Scene& scene)
{
    {
        auto floor = std::make_shared<Microfacet>(ImageTexture::Create("res/wakdu.jpg"), ConstantColor::Create(Spectrum(0.0f)),
                                                  ConstantColor::Create(Spectrum(0.0f)));
        Float s = 20.0f;
        auto tf = Transform{ Vec3(0.0f, -4.0f, -4.0f), identity, Vec3(s, 1.0f, s) };
        scene.Add(CreateRectXZ(tf, floor));

        tf = Transform{ Vec3(0.0f, -4.0f, -4.0f), identity, Vec3(s, s, 1.0f) };
        scene.Add(CreateRectXY(tf, floor));
    }

    // plates
    {
        auto m1 = std::make_shared<Microfacet>(ConstantColor::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                               ConstantColor::Create(Spectrum(1.0f)),
                                               ConstantColor::Create(Spectrum(std::sqrt(0.005f))));

        auto m2 = std::make_shared<Microfacet>(ConstantColor::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                               ConstantColor::Create(Spectrum(1.0f)),
                                               ConstantColor::Create(Spectrum(std::sqrt(0.02f))));

        auto m3 = std::make_shared<Microfacet>(ConstantColor::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                               ConstantColor::Create(Spectrum(1.0f)),
                                               ConstantColor::Create(Spectrum(std::sqrt(0.05f))));

        auto m4 =
            std::make_shared<Microfacet>(ConstantColor::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                         ConstantColor::Create(Spectrum(1.0f)), ConstantColor::Create(Spectrum(std::sqrt(0.1f))));

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
        auto light1 = std::make_shared<DiffuseLight>(Spectrum(800));
        auto light3 = std::make_shared<DiffuseLight>(Spectrum(901.803f, 0, 0));
        auto light2 = std::make_shared<DiffuseLight>(Spectrum(100, 100 / 5, 0));
        auto light4 = std::make_shared<DiffuseLight>(Spectrum(11.1111f, 11.1111f, 0));
        auto light5 = std::make_shared<DiffuseLight>(Spectrum(0, 1.23457f, 0));

        auto l1 = std::make_shared<Sphere>(Vec3(10, 10, 4), 0.5f, light1);
        auto l3 = std::make_shared<Sphere>(Vec3(-3.75f, 0, 0), 0.03333f, light3);
        auto l2 = std::make_shared<Sphere>(Vec3(-1.25f, 0, 0), 0.1f, light2);
        auto l4 = std::make_shared<Sphere>(Vec3(1.25f, 0, 0), 0.3f, light4);
        auto l5 = std::make_shared<Sphere>(Vec3(3.75f, 0, 0), 0.9f, light5);

        // scene.AddLight(l1);
        scene.AddLight(l2);
        scene.AddLight(l3);
        scene.AddLight(l4);
        scene.AddLight(l5);
    }

    Float aspect_ratio = 16. / 9.;
    // Float aspect_ratio = 3. / 2.;
    // Float aspect_ratio = 4. / 3.;
    // Float aspect_ratio = 1.;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0, 2, 15 };
    Point3 lookat{ 0, -2, 2.5f };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 28;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index2 = Sample::Register("mis-wak", MISTestWak);

} // namespace bulbit