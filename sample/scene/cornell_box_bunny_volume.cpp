#include "spt/pathtracer.h"

namespace spt
{

void CornellBoxBunnyVolume(Scene& scene)
{
    // Materials
    auto red = CreateSharedRef<Lambertian>(Color{ .65, .05, .05 });
    auto green = CreateSharedRef<Lambertian>(Color{ .12, .45, .15 });
    auto blue = CreateSharedRef<Lambertian>(Color{ .22, .23, .75 });
    auto white = CreateSharedRef<Lambertian>(Color{ .73, .73, .73 });
    auto skin = CreateSharedRef<Lambertian>(Color{ 251. / 255., 206. / 255., 177. / 255. });
    auto wakgood_texture = ImageTexture::Create("res/wakdu.jpg");
    auto wakgood_mat = CreateSharedRef<Lambertian>(wakgood_texture);
    auto light = CreateSharedRef<DiffuseLight>(Color{ 1.0 });
    auto glass = CreateSharedRef<Dielectric>(1.5);

    // Cornell box
    {
        // front
        auto tf = Transform{ Vec3{ 0.5, 0.5, -1.0 }, Quat{ identity }, Vec3{ 1.0 } };
        scene.Add(RectXY(tf, wakgood_mat));

        // left
        tf = Transform{ Vec3{ 0.0, 0.5, -0.5 }, Quat{ identity }, Vec3{ 1.0 } };
        scene.Add(RectYZ(tf, red));

        // right
        tf = Transform{ Vec3{ 1.0, 0.5, -0.5 }, Quat{ pi, y_axis }, Vec3{ 1.0 } };
        scene.Add(RectYZ(tf, green));

        // bottom
        tf = Transform{ Vec3{ 0.5, 0, -0.5 }, Quat{ identity }, Vec3{ 1.0 } };
        auto bottom = RectXZ(tf, white);
        scene.Add(bottom);

        // top
        tf = Transform{ Vec3{ 0.5, 1.0, -0.5 }, Quat{ pi, x_axis }, Vec3{ 1.0 } };
        scene.Add(RectXZ(tf, white));
    }

    // Lights
    {
        auto tf = Transform{ 0.5, 0.999, -0.5, Quat{ pi, x_axis }, Vec3{ 0.8 } };
        auto l = RectXZ(tf, light);

        scene.Add(l);
        scene.AddAreaLight(l->GetObjects()[0]);
        scene.AddAreaLight(l->GetObjects()[1]);
    }

    // Bunny
    {
        Vec3 center = Point3{ 0.5, 0.5, -0.5 };
        Transform tf{ center, Quat{ DegToRad(45), y_axis }, Vec3{ 0.5 } };
        auto m = Box(tf, white);
        // auto m = CreateSharedRef<Sphere>(tf.p, 0.3, white);

        // Vec3 center = Point3{ 0.5, 0.1, -0.5 };
        // Transform tf{ center, Quat{ DegToRad(0), x_axis }, Vec3{ 0.7 } };
        // Ref<Model> m = CreateSharedRef<Model>("res/stanford/bunny.obj", tf);

        auto volume = CreateSharedRef<ConstantDensityMedium>(m, 100.0, Color{ 1.0 });

        scene.Add(volume);
    }
}

} // namespace spt