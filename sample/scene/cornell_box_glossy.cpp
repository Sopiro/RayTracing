#include "bulbit/dielectric.h"
#include "bulbit/diffuse_light.h"
#include "bulbit/lambertian.h"
#include "bulbit/metal.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

void CornellBoxGlossy(Scene& scene)
{
    // Materials
    auto red = CreateSharedRef<Lambertian>(Spectrum(.65f, .05f, .05f));
    auto green = CreateSharedRef<Lambertian>(Spectrum(.12f, .45f, .15f));
    auto blue = CreateSharedRef<Lambertian>(Spectrum(.22f, .23f, .75f));
    auto white = CreateSharedRef<Lambertian>(Spectrum(.73f, .73f, .73f));
    auto wakgood_texture = ImageTexture::Create("res/wakdu.jpg");
    auto wakgood_mat = CreateSharedRef<Lambertian>(wakgood_texture);
    auto light = CreateSharedRef<DiffuseLight>(Spectrum(15.0f));
    auto mirror = CreateSharedRef<Metal>(Spectrum(.73f, .73f, .73f), 0.0f);
    auto glass = CreateSharedRef<Dielectric>(1.5f);

    // Cornell box
    {
        // front
        auto mat = CreateSharedRef<Microfacet>(wakgood_texture, ConstantColor::Create(Spectrum(0.0f)),
                                               ConstantColor::Create(Spectrum(0.01f)));

        auto tf = Transform{ Vec3(0.5f, 0.5f, -1.0f), identity, Vec3(1.0) };
        scene.Add(CreateRectXY(tf, mat));

        // left

        mat = CreateSharedRef<Microfacet>(ConstantColor::Create(.65f, .05f, .05f), ConstantColor::Create(Spectrum(0.0f)),
                                          ConstantColor::Create(Spectrum(0.01f)));
        tf = Transform{ Vec3(0.0f, 0.5f, -0.5f), identity, Vec3(1.0f) };
        scene.Add(CreateRectYZ(tf, mat));

        // right

        mat = CreateSharedRef<Microfacet>(ConstantColor::Create(.12f, .45f, .15f), ConstantColor::Create(Spectrum(0.0f)),
                                          ConstantColor::Create(Spectrum(0.01f)));
        tf = Transform{ Vec3(1.0f, 0.5f, -0.5f), Quat(pi, y_axis), Vec3(1.0f) };
        scene.Add(CreateRectYZ(tf, mat));

        // bottom
        mat = CreateSharedRef<Microfacet>(ConstantColor::Create(.73f), ConstantColor::Create(Spectrum(0.0f)),
                                          ConstantColor::Create(Spectrum(0.01f)));
        tf = Transform{ Vec3(0.5f, 0.0f, -0.5f), identity, Vec3(1.0f) };
        scene.Add(CreateRectXZ(tf, mat));

        // top
        mat = CreateSharedRef<Microfacet>(ConstantColor::Create(.73f), ConstantColor::Create(Spectrum(0.0f)),
                                          ConstantColor::Create(Spectrum(0.01f)));
        tf = Transform{ Vec3(0.5f, 1.0f, -0.5f), Quat(pi, x_axis), Vec3(1.0f) };
        scene.Add(CreateRectXZ(tf, mat));
    }

    // Left block
    {
        Float hx = 0.13f;
        Float hy = 0.26f;
        Float hz = 0.13f;

        auto mat = CreateSharedRef<Microfacet>(ConstantColor::Create(1.0f), ConstantColor::Create(Spectrum(1.0f)),
                                               ConstantColor::Create(Spectrum(0.1f)));
        auto tf = Transform{ 0.3f, hy, -0.6f, Quat(DegToRad(25.0f), y_axis), Vec3(hx * 2.0f, hy * 2.0f, hz * 2.0f) };
        auto box = CreateBox(tf, mat);

        scene.Add(box);
    }

    // Right bunny
    {
        auto mat = CreateSharedRef<Microfacet>(ConstantColor::Create(Spectrum(0.7f)), ConstantColor::Create(Spectrum(1.0f)),
                                               ConstantColor::Create(Spectrum(0.05f)));

        // Bunny
        Transform tf{ Point3(0.7f, 0.0f, -0.3f), identity, Vec3(0.3f) };

        Ref<Model> model = CreateSharedRef<Model>("res/stanford/bunny.obj", tf);
        model->GetMeshes()[0]->SetMaterial(mat);
        scene.Add(model);
    }

    // Lights
    {
        auto tf = Transform{ 0.5f, 0.999f, -0.5f, Quat(pi, x_axis), Vec3(0.25f) };
        auto l = CreateRectXZ(tf, light);

        scene.AddLight(l);
    }

    // scene.Rebuild();
    // std::cout << "Lights: " << scene.GetLights().GetCount() << std::endl;
}

} // namespace bulbit