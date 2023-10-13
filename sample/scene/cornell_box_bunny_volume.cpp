#include "spt/spt.h"

namespace spt
{

void CornellBoxBunnyVolume(Scene& scene)
{
    // Materials
    auto red = CreateSharedRef<Lambertian>(Spectrum(.65, .05, .05));
    auto green = CreateSharedRef<Lambertian>(Spectrum(.12, .45, .15));
    auto blue = CreateSharedRef<Lambertian>(Spectrum(.22, .23, .75));
    auto white = CreateSharedRef<Lambertian>(Spectrum(.73, .73, .73));
    auto skin = CreateSharedRef<Lambertian>(Spectrum(251. / 255., 206. / 255., 177. / 255.));
    auto wakgood_texture = ImageTexture::Create("res/wakdu.jpg");
    auto wakgood_mat = CreateSharedRef<Lambertian>(wakgood_texture);
    auto light = CreateSharedRef<DiffuseLight>(Spectrum(1.0));
    auto glass = CreateSharedRef<Dielectric>(1.5);

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

    // Lights
    {
        auto tf = Transform{ 0.5, 0.999, -0.5, Quat(pi, x_axis), Vec3(0.8) };
        auto l = CreateRectXZ(tf, light);

        scene.AddLight(l);
    }

    // Bunny
    {
        Point3 center(0.5, 0.5, -0.5);
        Transform tf{ center, Quat(DegToRad(45), y_axis), Vec3(0.5) };
        auto m = CreateBox(tf, white);

        // auto m = CreateSharedRef<Sphere>(tf.p, 0.3, white);

        // Vec3 center(0.5, 0.1, -0.5);
        // Transform tf{ center, Quat(DegToRad(0), x_axis), Vec3(0.7) };
        // Ref<Model> m = CreateSharedRef<Model>("res/stanford/bunny.obj", tf);

        auto a = CreateSharedRef<Aggregate>();
        a->Add(m);
        auto volume = CreateSharedRef<ConstantDensityMedium>(a, 100.0, Spectrum(1.0));

        scene.Add(volume);
    }
}

} // namespace spt