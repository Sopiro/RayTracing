#include "../samples.h"
#include "bulbit/camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

std::unique_ptr<Camera> MISTest(Scene& scene)
{
    {
        auto floor = Model("res/veach_mi/floor.obj", Transform{ identity });
        auto floor_mat = scene.CreateMaterial<Microfacet>(ConstantColor::Create(0.4f), ConstantColor::Create(Spectrum(0.0f)),
                                                          ConstantColor::Create(Spectrum(0.0f)));
        floor.GetMeshes()[0]->SetMaterial(floor_mat);
        scene.AddModel(floor);
    }

    // plates
    {
        auto m1 = scene.CreateMaterial<Microfacet>(ConstantColor::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                                   ConstantColor::Create(Spectrum(1.0f)),
                                                   ConstantColor::Create(Spectrum(std::sqrt(0.005f))));

        auto m2 = scene.CreateMaterial<Microfacet>(ConstantColor::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                                   ConstantColor::Create(Spectrum(1.0f)),
                                                   ConstantColor::Create(Spectrum(std::sqrt(0.02f))));

        auto m3 = scene.CreateMaterial<Microfacet>(ConstantColor::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                                   ConstantColor::Create(Spectrum(1.0f)),
                                                   ConstantColor::Create(Spectrum(std::sqrt(0.05f))));

        auto m4 = scene.CreateMaterial<Microfacet>(ConstantColor::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                                   ConstantColor::Create(Spectrum(1.0f)),
                                                   ConstantColor::Create(Spectrum(std::sqrt(0.1f))));

        auto p1 = Model("res/veach_mi/plate1.obj", Transform{ identity });
        scene.AddModel(p1);
        p1.GetMeshes()[0]->SetMaterial(m1);
        auto p2 = Model("res/veach_mi/plate2.obj", Transform{ identity });
        scene.AddModel(p2);
        p2.GetMeshes()[0]->SetMaterial(m2);
        auto p3 = Model("res/veach_mi/plate3.obj", Transform{ identity });
        scene.AddModel(p3);
        p3.GetMeshes()[0]->SetMaterial(m3);
        auto p4 = Model("res/veach_mi/plate4.obj", Transform{ identity });
        scene.AddModel(p4);
        p4.GetMeshes()[0]->SetMaterial(m4);
    }

    // Lights
    {
        auto light1 = scene.CreateMaterial<DiffuseLight>(Spectrum(800.f));
        auto light3 = scene.CreateMaterial<DiffuseLight>(Spectrum(901.803f));
        auto light2 = scene.CreateMaterial<DiffuseLight>(Spectrum(100.f));
        auto light4 = scene.CreateMaterial<DiffuseLight>(Spectrum(11.1111f));
        auto light5 = scene.CreateMaterial<DiffuseLight>(Spectrum(1.23457f));

        scene.CreatePrimitive<Sphere>(Vec3(10, 10, 4), 0.5f, light1);
        scene.CreatePrimitive<Sphere>(Vec3(-3.75f, 0, 0), 0.03333f, light3);
        scene.CreatePrimitive<Sphere>(Vec3(-1.25f, 0, 0), 0.1f, light2);
        scene.CreatePrimitive<Sphere>(Vec3(1.25f, 0, 0), 0.3f, light4);
        scene.CreatePrimitive<Sphere>(Vec3(3.75f, 0, 0), 0.9f, light5);
    }

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
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
        auto floor = scene.CreateMaterial<Microfacet>(
            ImageTexture::Create("res/wakdu.jpg"), ConstantColor::Create(Spectrum(0.0f)), ConstantColor::Create(Spectrum(0.0f)));
        Float s = 20.0f;
        auto tf = Transform{ Vec3(0.0f, -4.0f, -4.0f), identity, Vec3(s, 1.0f, s) };
        scene.AddMesh(CreateRectXZ(tf, floor));

        tf = Transform{ Vec3(0.0f, -4.0f, -4.0f), identity, Vec3(s, s, 1.0f) };
        scene.AddMesh(CreateRectXY(tf, floor));
    }

    // plates
    {
        auto m1 = scene.CreateMaterial<Microfacet>(ConstantColor::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                                   ConstantColor::Create(Spectrum(1.0f)),
                                                   ConstantColor::Create(Spectrum(std::sqrt(0.005f))));

        auto m2 = scene.CreateMaterial<Microfacet>(ConstantColor::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                                   ConstantColor::Create(Spectrum(1.0f)),
                                                   ConstantColor::Create(Spectrum(std::sqrt(0.02f))));

        auto m3 = scene.CreateMaterial<Microfacet>(ConstantColor::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                                   ConstantColor::Create(Spectrum(1.0f)),
                                                   ConstantColor::Create(Spectrum(std::sqrt(0.05f))));

        auto m4 = scene.CreateMaterial<Microfacet>(ConstantColor::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                                   ConstantColor::Create(Spectrum(1.0f)),
                                                   ConstantColor::Create(Spectrum(std::sqrt(0.1f))));

        auto p1 = Model("res/veach_mi/plate1.obj", Transform{ identity });
        scene.AddModel(p1);
        p1.GetMeshes()[0]->SetMaterial(m1);
        auto p2 = Model("res/veach_mi/plate2.obj", Transform{ identity });
        scene.AddModel(p2);
        p2.GetMeshes()[0]->SetMaterial(m2);
        auto p3 = Model("res/veach_mi/plate3.obj", Transform{ identity });
        scene.AddModel(p3);
        p3.GetMeshes()[0]->SetMaterial(m3);
        auto p4 = Model("res/veach_mi/plate4.obj", Transform{ identity });
        scene.AddModel(p4);
        p4.GetMeshes()[0]->SetMaterial(m4);
    }

    // Lights
    {
        auto light1 = scene.CreateMaterial<DiffuseLight>(Spectrum(800));
        auto light3 = scene.CreateMaterial<DiffuseLight>(Spectrum(901.803f, 0, 0));
        auto light2 = scene.CreateMaterial<DiffuseLight>(Spectrum(100, 100 / 5, 0));
        auto light4 = scene.CreateMaterial<DiffuseLight>(Spectrum(11.1111f, 11.1111f, 0));
        auto light5 = scene.CreateMaterial<DiffuseLight>(Spectrum(0, 1.23457f, 0));

        // scene.CreatePrimitive<Sphere>(Vec3(10, 10, 4), 0.5f, light1);
        scene.CreatePrimitive<Sphere>(Vec3(-3.75f, 0, 0), 0.03333f, light3);
        scene.CreatePrimitive<Sphere>(Vec3(-1.25f, 0, 0), 0.1f, light2);
        scene.CreatePrimitive<Sphere>(Vec3(1.25f, 0, 0), 0.3f, light4);
        scene.CreatePrimitive<Sphere>(Vec3(3.75f, 0, 0), 0.9f, light5);
    }

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
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