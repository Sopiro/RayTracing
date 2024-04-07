#include "../samples.h"
#include "bulbit/perspective_camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

std::unique_ptr<Camera> CornellBoxLucy(Scene& scene)
{
    // Materials
    auto red = scene.CreateMaterial<Lambertian>(Spectrum(.65f, .05f, .05f));
    auto green = scene.CreateMaterial<Lambertian>(Spectrum(.12f, .45f, .15f));
    auto blue = scene.CreateMaterial<Lambertian>(Spectrum(.22f, .23f, .75f));
    auto white = scene.CreateMaterial<Lambertian>(Spectrum(.73f, .73f, .73f));
    auto wakgood_texture = ImageTexture::Create("res/wakdu.jpg");
    auto wakgood_mat = scene.CreateMaterial<Lambertian>(wakgood_texture);
    auto light = scene.CreateMaterial<DiffuseLight>(Spectrum(15));

    // Cornell box
    {
        // front
        auto tf = Transform{ Vec3(0.5f, 0.5f, -1.0f), identity, Vec3(1.0f) };
        scene.AddMesh(CreateRectXY(tf, wakgood_mat));

        // left
        tf = Transform{ Vec3(0.0f, 0.5f, -0.5f), identity, Vec3(1.0f) };
        scene.AddMesh(CreateRectYZ(tf, red));

        // right
        tf = Transform{ Vec3(1.0f, 0.5f, -0.5f), Quat(pi, y_axis), Vec3(1.0f) };
        scene.AddMesh(CreateRectYZ(tf, green));

        // bottom
        tf = Transform{ Vec3(0.5f, 0.0f, -0.5f), identity, Vec3(1.0f) };
        scene.AddMesh(CreateRectXZ(tf, white));

        // top
        tf = Transform{ Vec3(0.5f, 1.0f, -0.5f), Quat(pi, x_axis), Vec3(1.0f) };
        scene.AddMesh(CreateRectXZ(tf, white));
    }

    // Lights
    {
        auto tf = Transform{ 0.5f, 0.999f, -0.5f, Quat(pi, x_axis), Vec3(0.25f) };
        auto l = CreateRectXZ(tf, light);

        scene.AddMesh(l);
    }

    {
        // Lucy
        Transform transform{ Point3(0.5f, 0.0f, -0.5f), identity, Vec3(0.85f) };
        auto mat = std::make_shared<Microfacet>(ConstantColor::Create(1.0f), ConstantColor::Create(Spectrum(1.0f)),
                                                ConstantColor::Create(Spectrum(0.2f)));

        // auto mat = scene.CreateMaterial<Dielectric>(1.5f);

        Material::fallback = mat;
        Model model = Model("res/stanford/lucy.obj", transform);
        scene.AddModel(model);
    }

    int32 width = 500;

    Point3 lookfrom{ 0.5f, 0.5f, 1.25f };
    Point3 lookat{ 0.5f, 0.5f, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 45;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, width, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("cornell-box-lucy", CornellBoxLucy);

} // namespace bulbit