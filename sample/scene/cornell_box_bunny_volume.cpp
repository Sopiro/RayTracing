#include "../samples.h"
#include "bulbit/constant_medium.h"
#include "bulbit/dielectric.h"
#include "bulbit/diffuse_light.h"
#include "bulbit/lambertian.h"
#include "bulbit/perspective_camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

Camera* ConstantVolume(Scene& scene)
{
    // Materials
    auto red = CreateSharedRef<Lambertian>(Spectrum(.65f, .05f, .05f));
    auto green = CreateSharedRef<Lambertian>(Spectrum(.12f, .45f, .15f));
    auto blue = CreateSharedRef<Lambertian>(Spectrum(.22f, .23f, .75f));
    auto white = CreateSharedRef<Lambertian>(Spectrum(.73f, .73f, .73f));
    auto skin = CreateSharedRef<Lambertian>(Spectrum(251.f / 255.f, 206.f / 255.f, 177.f / 255.f));
    auto wakgood_texture = ImageTexture::Create("res/wakdu.jpg");
    auto wakgood_mat = CreateSharedRef<Lambertian>(wakgood_texture);
    auto light = CreateSharedRef<DiffuseLight>(Spectrum(1.0f));
    auto glass = CreateSharedRef<Dielectric>(1.5f);

    // Cornell box
    {
        // front
        auto tf = Transform{ Vec3(0.5f, 0.5f, -1.0f), identity, Vec3(1.0f) };
        scene.Add(CreateRectXY(tf, wakgood_mat));

        // left
        tf = Transform{ Vec3(0.0f, 0.5f, -0.5f), identity, Vec3(1.0f) };
        scene.Add(CreateRectYZ(tf, red));

        // right
        tf = Transform{ Vec3(1.0f, 0.5f, -0.5f), Quat(pi, y_axis), Vec3(1.0f) };
        scene.Add(CreateRectYZ(tf, green));

        // bottom
        tf = Transform{ Vec3(0.5f, 0.0f, -0.5f), identity, Vec3(1.0f) };
        scene.Add(CreateRectXZ(tf, white));

        // top
        tf = Transform{ Vec3(0.5f, 1.0f, -0.5f), Quat(pi, x_axis), Vec3(1.0f) };
        scene.Add(CreateRectXZ(tf, white));
    }

    // Lights
    {
        auto tf = Transform{ 0.5f, 0.999f, -0.5f, Quat(pi, x_axis), Vec3(0.8f) };
        auto l = CreateRectXZ(tf, light);

        scene.AddLight(l);
    }

    // Bunny
    {
        Point3 center(0.5f, 0.5f, -0.5f);
        Transform tf{ center, Quat(DegToRad(45), y_axis), Vec3(0.5f) };
        auto m = CreateBox(tf, white);

        // auto m = CreateSharedRef<Sphere>(tf.p, 0.3f, white);

        // Vec3 center(0.5f, 0.1f, -0.5f);
        // Transform tf{ center, Quat(DegToRad(0), x_axis), Vec3(0.7f) };
        // Ref<Model> m = CreateSharedRef<Model>("res/stanford/bunny.obj", tf);

        auto a = CreateSharedRef<Aggregate>();
        a->Add(m);
        auto volume = CreateSharedRef<ConstantDensityMedium>(a, 100.0f, Spectrum(1.0f));

        scene.Add(volume);
    }

    int32 width = 500;

    Point3 lookfrom{ 0.5f, 0.5f, 1.25f };
    Point3 lookat{ 0.5f, 0.5f, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 45;

    return new PerspectiveCamera(lookfrom, lookat, y_axis, width, width, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("constant-volume", ConstantVolume);

} // namespace bulbit