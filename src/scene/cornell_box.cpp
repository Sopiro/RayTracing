#include "raytracer/raytracer.h"

namespace spt
{

void CornellBox(Scene& scene)
{
    auto red = std::make_shared<Lambertian>(Color{ .65, .05, .05 });
    auto green = std::make_shared<Lambertian>(Color{ .12, .45, .15 });
    auto blue = std::make_shared<Lambertian>(Color{ .22, .23, .75 });
    auto white = std::make_shared<Lambertian>(Color{ .73, .73, .73 });
    auto wakgood_texture = std::make_shared<ImageTexture>("res/wakdu.jpg");
    auto wakgood_mat = std::make_shared<Lambertian>(wakgood_texture);

    auto light = std::make_shared<DiffuseLight>(Color{ 15.0 });
    auto wakgood_light = std::make_shared<DiffuseLight>(wakgood_texture);

    Vertex v0{ Vec3{ 0.0, 0.0, 0.0 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 0.0, 0.0 } };
    Vertex v1{ Vec3{ 1.0, 0.0, 0.0 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 1.0, 0.0 } };
    Vertex v2{ Vec3{ 1.0, 1.0, 0.0 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 1.0, 1.0 } };
    Vertex v3{ Vec3{ 0.0, 1.0, 0.0 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 0.0, 1.0 } };

    Vertex v4{ Vec3{ 0.0, 0.0, -1.0 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 0.0, 0.0 } };
    Vertex v5{ Vec3{ 1.0, 0.0, -1.0 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 1.0, 0.0 } };
    Vertex v6{ Vec3{ 1.0, 1.0, -1.0 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 1.0, 1.0 } };
    Vertex v7{ Vec3{ 0.0, 1.0, -1.0 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 0.0, 1.0 } };

    // front
    scene.Add(std::make_shared<Triangle>(v4, v5, v6, wakgood_mat, true, true));
    scene.Add(std::make_shared<Triangle>(v4, v6, v7, wakgood_mat, true, true));

    // left
    scene.Add(std::make_shared<Triangle>(v0, v4, v7, red, true, true));
    scene.Add(std::make_shared<Triangle>(v0, v7, v3, red, true, true));

    // right
    scene.Add(std::make_shared<Triangle>(v5, v1, v2, green, true, true));
    scene.Add(std::make_shared<Triangle>(v5, v2, v6, green, true, true));

    // bottom
    scene.Add(std::make_shared<Triangle>(v0, v1, v5, white, true, true));
    scene.Add(std::make_shared<Triangle>(v0, v5, v4, white, true, true));

    // top
    scene.Add(std::make_shared<Triangle>(v7, v6, v2, white, true, true));
    scene.Add(std::make_shared<Triangle>(v7, v2, v3, white, true, true));

    // Transform tf1(Vec3{ 0.5, 0.8, -0.5 }, Quat{ DegToRad(-30.0), Vec3{ 1.0, 0.0, 0.0 } }, Vec3{ 0.25 });
    Transform tf1(Vec3{ 0.5, 0.999, -0.5 }, Quat{ identity }, Vec3{ 0.25 });
    Vertex t0{ tf1 * Vec3{ -0.5, 0.0, 0.5 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 0.0, 0.0 } };
    Vertex t1{ tf1 * Vec3{ 0.5, 0.0, 0.5 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 1.0, 0.0 } };
    Vertex t2{ tf1 * Vec3{ 0.5, 0.0, -0.5 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 1.0, 1.0 } };
    Vertex t3{ tf1 * Vec3{ -0.5, 0.0, -0.5 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 0.0, 1.0 } };

    Transform tf2(Vec3{ 0.1, 0.999, -0.1 }, Quat{ identity }, Vec3{ 0.25 });
    Vertex u0{ tf2 * Vec3{ -0.5, 0.0, 0.5 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 0.0, 0.0 } };
    Vertex u1{ tf2 * Vec3{ 0.5, 0.0, 0.5 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 1.0, 0.0 } };
    Vertex u2{ tf2 * Vec3{ 0.5, 0.0, -0.5 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 1.0, 1.0 } };
    Vertex u3{ tf2 * Vec3{ -0.5, 0.0, -0.5 }, Vec3{ 0.0, 0.0, 0.0 }, Vec2{ 0.0, 1.0 } };

    // Left block
    {
        double hx = 0.13;
        double hy = 0.26;
        double hz = 0.13;

        // double w = 0.02;
        double w = 0.0;

        Transform t(0.3, hy + w, -0.6, Quat(DegToRad(25.0), Vec3(0.0, 1.0, 0.0)));

        Vertex v0{ t * Vec3{ -hx, -hy, hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 0.0, 0.0 } };
        Vertex v1{ t * Vec3{ hx, -hy, hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 1.0, 0.0 } };
        Vertex v2{ t * Vec3{ hx, hy, hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 1.0, 1.0 } };
        Vertex v3{ t * Vec3{ -hx, hy, hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 0.0, 1.0 } };

        Vertex v4{ t * Vec3{ -hx, -hy, -hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 0.0, 0.0 } };
        Vertex v5{ t * Vec3{ hx, -hy, -hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 1.0, 0.0 } };
        Vertex v6{ t * Vec3{ hx, hy, -hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 1.0, 1.0 } };
        Vertex v7{ t * Vec3{ -hx, hy, -hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 0.0, 1.0 } };

        // auto mat = std::make_shared<Metal>(Color{ 0.6, 0.6, 0.6 }, 0.2);
        // auto mat = std::make_shared<Metal>(Color{ 0.6, 0.6, 0.6 }, 0.0);
        // auto mat = std::make_shared<Dielectric>(2.4);
        auto mat = white;

        std::shared_ptr<HittableList> boundary = std::make_shared<HittableList>();

        // front
        boundary->Add(std::make_shared<Triangle>(v0, v1, v2, mat, true, true));
        boundary->Add(std::make_shared<Triangle>(v0, v2, v3, mat, true, true));

        // right
        boundary->Add(std::make_shared<Triangle>(v1, v5, v6, mat, true, true));
        boundary->Add(std::make_shared<Triangle>(v1, v6, v2, mat, true, true));

        // back
        boundary->Add(std::make_shared<Triangle>(v5, v4, v7, mat, true, true));
        boundary->Add(std::make_shared<Triangle>(v5, v7, v6, mat, true, true));

        // left
        boundary->Add(std::make_shared<Triangle>(v4, v0, v3, mat, true, true));
        boundary->Add(std::make_shared<Triangle>(v4, v3, v7, mat, true, true));

        // top
        boundary->Add(std::make_shared<Triangle>(v3, v2, v6, mat, true, true));
        boundary->Add(std::make_shared<Triangle>(v3, v6, v7, mat, true, true));

        // bottom
        boundary->Add(std::make_shared<Triangle>(v1, v0, v4, mat, true, true));
        boundary->Add(std::make_shared<Triangle>(v1, v4, v5, mat, true, true));

        // scene.Add(std::make_shared<ConstantDensityMedium>(boundary, 4.5, Color(1.0)));
        scene.Add(boundary);
    }

    // lights
    {
        auto l1 = std::make_shared<Triangle>(t0, t2, t1, light, true, true);
        auto l2 = std::make_shared<Triangle>(t0, t3, t2, light, true, true);

        auto l3 = std::make_shared<Triangle>(u0, u2, u1, light, true, true);
        auto l4 = std::make_shared<Triangle>(u0, u3, u2, light, true, true);

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