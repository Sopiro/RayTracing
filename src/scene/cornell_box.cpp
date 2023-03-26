#include "raytracer/raytracer.h"

namespace spt
{

void CornellBox(Scene& scene)
{
    auto red = std::make_shared<Lambertian>(Color{ .65, .05, .05 });
    auto green = std::make_shared<Lambertian>(Color{ .12, .45, .15 });
    auto blue = std::make_shared<Lambertian>(Color{ .22, .23, .75 });
    auto white = std::make_shared<Lambertian>(Color{ .73, .73, .73 });
    auto wakgood_texture = ImageTexture::Create("res/wakdu.jpg");
    auto wakgood_mat = std::make_shared<Lambertian>(wakgood_texture);
    auto light = std::make_shared<DiffuseLight>(Color{ 15.0 });

    Vec3 v0{ 0.0, 0.0, 0.0 };
    Vec3 v1{ 1.0, 0.0, 0.0 };
    Vec3 v2{ 1.0, 1.0, 0.0 };
    Vec3 v3{ 0.0, 1.0, 0.0 };

    Vec3 v4{ 0.0, 0.0, -1.0 };
    Vec3 v5{ 1.0, 0.0, -1.0 };
    Vec3 v6{ 1.0, 1.0, -1.0 };
    Vec3 v7{ 0.0, 1.0, -1.0 };

    {
        // front
        auto t1 = std::make_shared<Triangle>(v4, v5, v6, wakgood_mat);
        auto t2 = std::make_shared<Triangle>(v4, v6, v7, wakgood_mat);
        t1->v0.texCoords.Set(0.0, 0.0);
        t1->v1.texCoords.Set(1.0, 0.0);
        t1->v2.texCoords.Set(1.0, 1.0);
        t2->v0.texCoords.Set(0.0, 0.0);
        t2->v1.texCoords.Set(1.0, 1.0);
        t2->v2.texCoords.Set(0.0, 1.0);

        scene.Add(t1);
        scene.Add(t2);

        // left
        scene.Add(std::make_shared<Triangle>(v0, v4, v7, red));
        scene.Add(std::make_shared<Triangle>(v0, v7, v3, red));

        // right
        scene.Add(std::make_shared<Triangle>(v5, v1, v2, green));
        scene.Add(std::make_shared<Triangle>(v5, v2, v6, green));

        // bottom
        scene.Add(std::make_shared<Triangle>(v0, v1, v5, white));
        scene.Add(std::make_shared<Triangle>(v0, v5, v4, white));

        // top
        scene.Add(std::make_shared<Triangle>(v7, v6, v2, white));
        scene.Add(std::make_shared<Triangle>(v7, v2, v3, white));
    }

    // Transform tf1(Vec3{ 0.5, 0.8, -0.5 }, Quat{ DegToRad(-30.0), Vec3{ 1.0, 0.0, 0.0 } }, Vec3{ 0.25 });
    Transform tf1{ Vec3{ 0.5, 0.999, -0.5 }, Quat{ identity }, Vec3{ 0.25 } };
    Vec3 t0 = tf1 * Vec3{ -0.5, 0.0, 0.5 };
    Vec3 t1 = tf1 * Vec3{ 0.5, 0.0, 0.5 };
    Vec3 t2 = tf1 * Vec3{ 0.5, 0.0, -0.5 };
    Vec3 t3 = tf1 * Vec3{ -0.5, 0.0, -0.5 };

    Transform tf2{ Vec3{ 0.1, 0.999, -0.1 }, Quat{ identity }, Vec3{ 0.25 } };
    Vec3 u0 = tf2 * Vec3{ -0.5, 0.0, 0.5 };
    Vec3 u1 = tf2 * Vec3{ 0.5, 0.0, 0.5 };
    Vec3 u2 = tf2 * Vec3{ 0.5, 0.0, -0.5 };
    Vec3 u3 = tf2 * Vec3{ -0.5, 0.0, -0.5 };

    // Left block
    {
        double hx = 0.13;
        double hy = 0.26;
        double hz = 0.13;

        // double w = 0.02;
        double w = 0.0;

        Transform t{ 0.3, hy + w, -0.6, Quat(DegToRad(25.0), Vec3(0.0, 1.0, 0.0)) };

        Vec3 v0 = t * Vec3{ -hx, -hy, hz };
        Vec3 v1 = t * Vec3{ hx, -hy, hz };
        Vec3 v2 = t * Vec3{ hx, hy, hz };
        Vec3 v3 = t * Vec3{ -hx, hy, hz };

        Vec3 v4 = t * Vec3{ -hx, -hy, -hz };
        Vec3 v5 = t * Vec3{ hx, -hy, -hz };
        Vec3 v6 = t * Vec3{ hx, hy, -hz };
        Vec3 v7 = t * Vec3{ -hx, hy, -hz };

        // auto mat = std::make_shared<Metal>(Color{ 0.6, 0.6, 0.6 }, 0.2);
        // auto mat = std::make_shared<Metal>(Color{ 0.6, 0.6, 0.6 }, 0.0);
        // auto mat = std::make_shared<Dielectric>(2.4);
        auto mat = white;

        std::shared_ptr<HittableList> boundary = std::make_shared<HittableList>();

        // front
        boundary->Add(std::make_shared<Triangle>(v0, v1, v2, mat));
        boundary->Add(std::make_shared<Triangle>(v0, v2, v3, mat));

        // right
        boundary->Add(std::make_shared<Triangle>(v1, v5, v6, mat));
        boundary->Add(std::make_shared<Triangle>(v1, v6, v2, mat));

        // back
        boundary->Add(std::make_shared<Triangle>(v5, v4, v7, mat));
        boundary->Add(std::make_shared<Triangle>(v5, v7, v6, mat));

        // left
        boundary->Add(std::make_shared<Triangle>(v4, v0, v3, mat));
        boundary->Add(std::make_shared<Triangle>(v4, v3, v7, mat));

        // top
        boundary->Add(std::make_shared<Triangle>(v3, v2, v6, mat));
        boundary->Add(std::make_shared<Triangle>(v3, v6, v7, mat));

        // bottom
        boundary->Add(std::make_shared<Triangle>(v1, v0, v4, mat));
        boundary->Add(std::make_shared<Triangle>(v1, v4, v5, mat));

        // scene.Add(std::make_shared<ConstantDensityMedium>(boundary, 4.5, Color(1.0)));
        scene.Add(boundary);
    }

    // lights
    {
        auto l1 = std::make_shared<Triangle>(t0, t2, t1, light);
        auto l2 = std::make_shared<Triangle>(t0, t3, t2, light);

        auto l3 = std::make_shared<Triangle>(u0, u2, u1, light);
        auto l4 = std::make_shared<Triangle>(u0, u3, u2, light);

        scene.Add(l1);
        scene.Add(l2);

        scene.AddLight(l1);
        scene.AddLight(l2);
    }

    // Right sphere
    {
        auto mat = std::make_shared<Dielectric>(1.5);
        auto sphere = std::make_shared<Sphere>(Vec3(0.65, 0.15, -0.3), 0.15, mat);
        scene.Add(sphere);

        scene.AddLight(sphere);
    }

    scene.SetSkyColor(Color{ 0.0, 0.0, 0.0 });
}

} // namespace spt