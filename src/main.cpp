#if defined(_WIN32) && defined(_DEBUG)
#include <crtdbg.h>
#endif

#include "raytracer/raytracer.h"

using namespace spt;

void RandomScene(Scene& scene)
{
    auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    scene.Add(std::make_shared<Sphere>(Vec3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = Rand();
            Vec3 center(a + 0.9 * Rand(), 0.2, b + 0.9 * Rand());

            if ((center - Vec3(4, 0.2, 0)).Length() > 0.9)
            {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto albedo = Color::Random() * Color::Random();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    scene.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = Color::Random(0.5, 1);
                    auto fuzz = Rand(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    scene.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    scene.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    scene.Add(std::make_shared<Sphere>(Vec3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    scene.Add(std::make_shared<Sphere>(Vec3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    scene.Add(std::make_shared<Sphere>(Vec3(4, 1, 0), 1.0, material3));
}

void TestScene(Scene& scene)
{
    auto material_ground = std::make_shared<Lambertian>(Color{ 0.8, 0.8, 0.8 });
    auto material_center = std::make_shared<Lambertian>(Color{ 0.1, 0.2, 0.5 });
    auto material_left = std::make_shared<Dielectric>(1.5);
    auto material_right = std::make_shared<Metal>(Color{ 0.8, 0.6, 0.2 }, 0.0);
    auto checkerTexture = std::make_shared<CheckerTexture>(Color{ 0.2, 0.3, 0.1 }, Color{ 0.9, 0.9, 0.9 });
    auto checker = std::make_shared<Lambertian>(checkerTexture);

    auto light = std::make_shared<DiffuseLight>(Vec3{ 3.0 });

    scene.Add(std::make_shared<Sphere>(Vec3{ 0.0, -100.5, -1.0 }, 100.0, checker));
    // world.add(std::make_shared<Sphere>(Vec3{ 0.0, 0.0, -1.0 }, 0.5, material_center));
    scene.Add(std::make_shared<Sphere>(Vec3{ -1.0, 0.0, -1.0 }, 0.5, material_left));
    scene.Add(std::make_shared<Sphere>(Vec3(-1.0, 0.0, -1.0), -0.45, material_left));
    scene.Add(std::make_shared<Sphere>(Vec3{ 1.0, 0.0, -1.0 }, 0.5, material_right));

    scene.Add(std::make_shared<Sphere>(Vec3{ 0.0, 2.0, 0.0 }, 0.5, light));
    scene.Add(std::make_shared<Sphere>(Vec3{ 5.0, 2.0, -5.0 }, 0.5, light));
    scene.Add(std::make_shared<Sphere>(Vec3{ -5.0, 2.0, -5.0 }, 0.5, light));

    auto smoke = std::make_shared<Sphere>(Vec3{ 0.0, 0.0, -1.0 }, 0.5, material_center);
    scene.Add(std::make_shared<ConstantDensityMedium>(smoke, 2.0, Color(0.0)));

    auto fog = std::make_shared<Sphere>(Vec3{ 0.0, 0.0, -1.0 }, 3.0, material_center);
    scene.Add(std::make_shared<ConstantDensityMedium>(fog, 0.05, Color(1.0)));
}

void CornellBox(Scene& scene)
{
    auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    auto black = std::make_shared<Lambertian>(Color(0.0));
    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    auto glass = std::make_shared<Dielectric>(1.5);
    auto metal = std::make_shared<Metal>(Color{ 0.6, 0.6, 0.6 }, 0.0);
    auto light = std::make_shared<DiffuseLight>(Color(12.0));
    auto absorb = std::make_shared<DiffuseLight>(Color(0.0));

    double r = 1.0e5;
    double g = 1;
    double m = g / 2.0;

    scene.Add(std::make_shared<Sphere>(Vec3{ -r, m, m }, r, green));        // left
    scene.Add(std::make_shared<Sphere>(Vec3{ r + g, m, m }, r, red));       // right
    scene.Add(std::make_shared<Sphere>(Vec3{ m, m, -r }, r, white));        // front
    scene.Add(std::make_shared<Sphere>(Vec3{ m, m, r + 2.41 }, r, absorb)); // back
    scene.Add(std::make_shared<Sphere>(Vec3{ m, -r, m }, r, white));        // bottom
    scene.Add(std::make_shared<Sphere>(Vec3{ m, r + g, m }, r, white));     // top

    scene.Add(std::make_shared<Sphere>(Vec3{ m, 10.0, m }, 9.003, light)); // light

    scene.Add(std::make_shared<Sphere>(Vec3{ 0.8, 0.13, 0.5 }, 0.13, glass));
    scene.Add(std::make_shared<Sphere>(Vec3{ 0.3, 0.18, 0.8 }, 0.18, metal));
    // objects.add(std::make_shared<Sphere>(Vec3{ 0.3, 0.2, 0.7 }, -0.19, glass));
}

void WaknellBox(Scene& scene)
{
    auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    auto black = std::make_shared<Lambertian>(Color(0.0));
    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    auto glass = std::make_shared<Dielectric>(1.5);
    auto metal = std::make_shared<Metal>(Color{ 0.6, 0.6, 0.6 }, 0.0);
    auto light = std::make_shared<DiffuseLight>(Color(12.0));
    auto absorb = std::make_shared<DiffuseLight>(Color(0.0));

    double r = 1.0e5;
    double g = 1;
    double m = g / 2.0;

    scene.Add(std::make_shared<Sphere>(Vec3{ -r, m, m }, r, red));      // left
    scene.Add(std::make_shared<Sphere>(Vec3{ r + g, m, m }, r, green)); // right

    { // front

        auto wakgood_texture = std::make_shared<ImageTexture>("res/wakdu.jpg");
        auto wakgood_mat = std::make_shared<Lambertian>(wakgood_texture);

        Vertex v0{ Vec3{ 0.0, 0.0, 0.0 }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 0.0, 0.0 } };
        Vertex v1{ Vec3{ 1.0, 0.0, 0.0 }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 1.0, 0.0 } };
        Vertex v2{ Vec3{ 1.0, 1.0, 0.0 }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 1.0, 1.0 } };
        Vertex v3{ Vec3{ 0.0, 1.0, 0.0 }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 0.0, 1.0 } };
        scene.Add(std::make_shared<Triangle>(v0, v1, v2, wakgood_mat));
        scene.Add(std::make_shared<Triangle>(v0, v2, v3, wakgood_mat));
    }

    scene.Add(std::make_shared<Sphere>(Vec3{ m, m, r + 2.41 }, r, absorb)); // back
    scene.Add(std::make_shared<Sphere>(Vec3{ m, -r, m }, r, white));        // bottom
    scene.Add(std::make_shared<Sphere>(Vec3{ m, r + g, m }, r, white));     // top

    scene.Add(std::make_shared<Sphere>(Vec3{ m, 10.0, m }, 9.003, light)); // light

    // scene.Add(std::make_shared<Sphere>(Vec3{ 0.7, 0.13, 0.7 }, 0.13, metal));
    // scene.Add(std::make_shared<Sphere>(Vec3{ 0.3, 0.18, 0.8 }, 0.18, metal));

    {
        double hx = 0.115;
        double hy = 0.25;
        double hz = 0.115;

        Transform t(0.25, hy + 0.02, 0.4, Quat(DegToRad(20.0), Vec3(0.0, 1.0, 0.0)));

        Vertex v0{ t * Vec3{ -hx, -hy, hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 0.0, 0.0 } };
        Vertex v1{ t * Vec3{ hx, -hy, hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 1.0, 0.0 } };
        Vertex v2{ t * Vec3{ hx, hy, hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 1.0, 1.0 } };
        Vertex v3{ t * Vec3{ -hx, hy, hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 0.0, 1.0 } };

        Vertex v4{ t * Vec3{ -hx, -hy, -hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 0.0, 0.0 } };
        Vertex v5{ t * Vec3{ hx, -hy, -hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 1.0, 0.0 } };
        Vertex v6{ t * Vec3{ hx, hy, -hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 1.0, 1.0 } };
        Vertex v7{ t * Vec3{ -hx, hy, -hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 0.0, 1.0 } };

        // auto mat = std::make_shared<Metal>(Color{ 0.6, 0.6, 0.6 }, 0.2);
        // auto mat = std::make_shared<Dielectric>(2.4);
        auto mat = white;

        // front
        scene.Add(std::make_shared<Triangle>(v0, v1, v2, mat, true, true));
        scene.Add(std::make_shared<Triangle>(v0, v2, v3, mat, true, true));

        // right
        scene.Add(std::make_shared<Triangle>(v1, v5, v6, mat, true, true));
        scene.Add(std::make_shared<Triangle>(v1, v6, v2, mat, true, true));

        // back
        scene.Add(std::make_shared<Triangle>(v5, v4, v7, mat, true, true));
        scene.Add(std::make_shared<Triangle>(v5, v7, v6, mat, true, true));

        // left
        scene.Add(std::make_shared<Triangle>(v4, v0, v3, mat, true, true));
        scene.Add(std::make_shared<Triangle>(v4, v3, v7, mat, true, true));

        // top
        scene.Add(std::make_shared<Triangle>(v3, v2, v6, mat, true, true));
        scene.Add(std::make_shared<Triangle>(v3, v6, v7, mat, true, true));

        // bottom
        scene.Add(std::make_shared<Triangle>(v1, v0, v4, mat, true, true));
        scene.Add(std::make_shared<Triangle>(v1, v4, v5, mat, true, true));
    }

    {
        double hx = 0.12;
        double hy = 0.12;
        double hz = 0.12;

        // Transform t(0.7, 0.2, 0.7, Quat(DegToRad(30.0), Vec3(0.0, 0.0, 1.0)) * Quat(DegToRad(30.0), Vec3(0.0, 1.0, 0.0)));
        Transform t(0.7, hy, 0.7, Quat(DegToRad(-30.0), Vec3(0.0, 1.0, 0.0)));

        Vertex v0{ t * Vec3{ -hx, -hy, hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 0.0, 0.0 } };
        Vertex v1{ t * Vec3{ hx, -hy, hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 1.0, 0.0 } };
        Vertex v2{ t * Vec3{ hx, hy, hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 1.0, 1.0 } };
        Vertex v3{ t * Vec3{ -hx, hy, hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 0.0, 1.0 } };

        Vertex v4{ t * Vec3{ -hx, -hy, -hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 0.0, 0.0 } };
        Vertex v5{ t * Vec3{ hx, -hy, -hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 1.0, 0.0 } };
        Vertex v6{ t * Vec3{ hx, hy, -hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 1.0, 1.0 } };
        Vertex v7{ t * Vec3{ -hx, hy, -hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 0.0, 1.0 } };

        // auto mat = std::make_shared<Dielectric>(1.5);
        // auto mat = std::make_shared<Dielectric>(2.0);
        // auto mat = std::make_shared<Metal>(Color{ 0.6, 0.6, 0.6 }, 0.1);

        auto mat = white;

        // front
        scene.Add(std::make_shared<Triangle>(v0, v1, v2, mat, true, true));
        scene.Add(std::make_shared<Triangle>(v0, v2, v3, mat, true, true));

        // right
        scene.Add(std::make_shared<Triangle>(v1, v5, v6, mat, true, true));
        scene.Add(std::make_shared<Triangle>(v1, v6, v2, mat, true, true));

        // back
        scene.Add(std::make_shared<Triangle>(v5, v4, v7, mat, true, true));
        scene.Add(std::make_shared<Triangle>(v5, v7, v6, mat, true, true));

        // left
        scene.Add(std::make_shared<Triangle>(v4, v0, v3, mat, true, true));
        scene.Add(std::make_shared<Triangle>(v4, v3, v7, mat, true, true));

        // top
        scene.Add(std::make_shared<Triangle>(v3, v2, v6, mat, true, true));
        scene.Add(std::make_shared<Triangle>(v3, v6, v7, mat, true, true));

        // bottom
        scene.Add(std::make_shared<Triangle>(v1, v0, v4, mat, true, true));
        scene.Add(std::make_shared<Triangle>(v1, v4, v5, mat, true, true));
    }
}

void TriangleTest(Scene& scene)
{
    auto gray = std::make_shared<Lambertian>(Color{ 0.8, 0.8, 0.8 });
    auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    auto blue = std::make_shared<Lambertian>(Color{ .22, .23, .75 });
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    auto black = std::make_shared<Lambertian>(Color(0.0));
    auto glass = std::make_shared<Dielectric>(1.5);
    auto metal = std::make_shared<Metal>(Color{ 0.6, 0.6, 0.6 }, 0.0);
    auto light = std::make_shared<DiffuseLight>(Color(12.0));
    auto absorb = std::make_shared<DiffuseLight>(Color(0.0));
    auto checkerTexture = std::make_shared<CheckerTexture>(Color{ 0.2, 0.3, 0.1 }, Color{ 0.9, 0.9, 0.9 });
    auto checker = std::make_shared<Lambertian>(checkerTexture);

    auto earth_texture = std::make_shared<ImageTexture>("res/earthmap.jpg");
    auto earth_mat = std::make_shared<Lambertian>(earth_texture);
    auto whitted_texture = std::make_shared<ImageTexture>("res/wakdu.jpg");
    auto whitted_mat = std::make_shared<Lambertian>(whitted_texture);

    scene.Add(std::make_shared<Sphere>(Vec3{ 0.0, 2.0, 0.0 }, 0.5, light));
    scene.Add(std::make_shared<Sphere>(Vec3{ 5.0, 2.0, -5.0 }, 0.5, light));
    scene.Add(std::make_shared<Sphere>(Vec3{ -5.0, 2.0, -5.0 }, 0.5, light));

    // Transform t(0, 0, -1, Quat(pi / 4, Vec3(0, 0, 1)));
    Transform t(0, 0, -1, Quat(0.0, Vec3(0, 0, 1)));

    Vec3 p0 = t * Vec3{ -0.5, 0.0, 0.0 };
    Vec3 p1 = t * Vec3{ 0.5, 0.0, 0.0 };
    Vec3 p2 = t * Vec3{ 0.0, 1.0, 0.0 };

    Vertex v0{ p0, Vec3{ 0, 0, 1 }, Vec2{ 0.0, 0.0 } };
    Vertex v1{ p1, Vec3{ 0, 0, 1 }, Vec2{ 1.0, 0.0 } };
    Vertex v2{ p2, Vec3{ 0, 0, 1 }, Vec2{ 0.5, 1.0 } };

    scene.Add(std::make_shared<Triangle>(v0, v1, v2, whitted_mat));

    double p = 5.0;
    double y = -0.5;
    // objects.Add(std::make_shared<Triangle>(Vec3{ -p, y, -p }, Vec3{ p, y, -p }, Vec3{ p, y, p }, gray));
    // objects.Add(std::make_shared<Triangle>(Vec3{ -p, y, -p }, Vec3{ p, y, p }, Vec3{ -p, y, p }, gray));
    scene.Add(std::make_shared<Sphere>(Vec3{ 1.0, 0.0, -1.5 }, 0.5, earth_mat));
    scene.Add(std::make_shared<Sphere>(Vec3{ -1.0, 0.0, -0.5 }, 0.3, earth_mat));
    scene.Add(std::make_shared<Sphere>(Vec3{ -1.0, 0.0, -0.5 }, 0.5, glass));
}

void BVHTest(Scene& scene)
{
    auto gray = std::make_shared<Lambertian>(Color{ 0.8, 0.8, 0.8 });
    auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    auto blue = std::make_shared<Lambertian>(Color{ .22, .23, .75 });
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    auto black = std::make_shared<Lambertian>(Color(0.0));
    auto glass = std::make_shared<Dielectric>(1.5);
    auto metal = std::make_shared<Metal>(Color{ 0.6, 0.6, 0.6 }, 0.0);
    auto light = std::make_shared<DiffuseLight>(Color(12.0));
    auto absorb = std::make_shared<DiffuseLight>(Color(0.0));
    auto checkerTexture = std::make_shared<CheckerTexture>(Color{ 0.2, 0.3, 0.1 }, Color{ 0.9, 0.9, 0.9 });
    auto checker = std::make_shared<Lambertian>(checkerTexture);

    double n = 30.0;
    double w = 7.0;
    double h = w * 9.0 / 16.0;
    double r = 0.1;

    for (int32 y = 0; y < n; ++y)
    {
        for (int32 x = 0; x < n; ++x)
        {
            Vec3 pos;
            // pos.x = x / n * w - w / 2.0;
            // pos.y = y / n * w - w / 2.0;

            pos.x = Prand(-w, w);
            pos.y = Prand(-h, h);
            pos.z = -1;

            scene.Add(std::make_shared<Sphere>(pos, r, green));
        }
    }
}

void Sponza(Scene& scene)
{
    Transform transform{ Vec3{ 0.0, 0.0, 0.0 }, Quat{ DegToRad(90.0), Vec3{ 0.0, 1.0, 0.0 } } };

    std::shared_ptr<Model> sponza = std::make_shared<Model>("res/sponza/Sponza.gltf", transform);

    scene.Add(sponza);
}

std::shared_ptr<Hittable> CornellBox2(Scene& scene)
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

    // lights
    std::shared_ptr<Hittable> l1 = std::make_shared<Triangle>(t0, t2, t1, light, true, true);
    std::shared_ptr<Hittable> l2 = std::make_shared<Triangle>(t0, t3, t2, light, true, true);

    std::shared_ptr<Hittable> l3 = std::make_shared<Triangle>(u0, u2, u1, light, true, true);
    std::shared_ptr<Hittable> l4 = std::make_shared<Triangle>(u0, u3, u2, light, true, true);

    std::shared_ptr<Scene> lights = std::make_shared<Scene>();
    lights->Add(l1);
    lights->Add(l2);

    scene.Add(l1);
    scene.Add(l2);

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

        // front
        scene.Add(std::make_shared<Triangle>(v0, v1, v2, mat, true, true));
        scene.Add(std::make_shared<Triangle>(v0, v2, v3, mat, true, true));

        // right
        scene.Add(std::make_shared<Triangle>(v1, v5, v6, mat, true, true));
        scene.Add(std::make_shared<Triangle>(v1, v6, v2, mat, true, true));

        // back
        scene.Add(std::make_shared<Triangle>(v5, v4, v7, mat, true, true));
        scene.Add(std::make_shared<Triangle>(v5, v7, v6, mat, true, true));

        // left
        scene.Add(std::make_shared<Triangle>(v4, v0, v3, mat, true, true));
        scene.Add(std::make_shared<Triangle>(v4, v3, v7, mat, true, true));

        // top
        scene.Add(std::make_shared<Triangle>(v3, v2, v6, mat, true, true));
        scene.Add(std::make_shared<Triangle>(v3, v6, v7, mat, true, true));

        // bottom
        scene.Add(std::make_shared<Triangle>(v1, v0, v4, mat, true, true));
        scene.Add(std::make_shared<Triangle>(v1, v4, v5, mat, true, true));
    }

    // Right block
    // {
    //     double hx = 0.12;
    //     double hy = 0.12;
    //     double hz = 0.12;

    //     // Transform t(0.7, 0.2, 0.7, Quat(DegToRad(30.0), Vec3(0.0, 0.0, 1.0)) * Quat(DegToRad(30.0), Vec3(0.0, 1.0, 0.0)));
    //     Transform t(0.7, hy, -0.3, Quat(DegToRad(-30.0), Vec3(0.0, 1.0, 0.0)));

    //     Vertex v0{ t * Vec3{ -hx, -hy, hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 0.0, 0.0 } };
    //     Vertex v1{ t * Vec3{ hx, -hy, hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 1.0, 0.0 } };
    //     Vertex v2{ t * Vec3{ hx, hy, hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 1.0, 1.0 } };
    //     Vertex v3{ t * Vec3{ -hx, hy, hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 0.0, 1.0 } };

    //     Vertex v4{ t * Vec3{ -hx, -hy, -hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 0.0, 0.0 } };
    //     Vertex v5{ t * Vec3{ hx, -hy, -hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 1.0, 0.0 } };
    //     Vertex v6{ t * Vec3{ hx, hy, -hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 1.0, 1.0 } };
    //     Vertex v7{ t * Vec3{ -hx, hy, -hz }, Vec3{ 0.0, 0.0, 1.0 }, Vec2{ 0.0, 1.0 } };

    //     // auto mat = std::make_shared<Dielectric>(1.5);
    //     // auto mat = std::make_shared<Dielectric>(2.0);
    //     auto mat = std::make_shared<Metal>(Color{ 0.6, 0.6, 0.6 }, 0.1);

    //     // auto mat = white;

    //     // front
    //     scene.Add(std::make_shared<Triangle>(v0, v1, v2, mat, true, true));
    //     scene.Add(std::make_shared<Triangle>(v0, v2, v3, mat, true, true));

    //     // right
    //     scene.Add(std::make_shared<Triangle>(v1, v5, v6, mat, true, true));
    //     scene.Add(std::make_shared<Triangle>(v1, v6, v2, mat, true, true));

    //     // back
    //     scene.Add(std::make_shared<Triangle>(v5, v4, v7, mat, true, true));
    //     scene.Add(std::make_shared<Triangle>(v5, v7, v6, mat, true, true));

    //     // left
    //     scene.Add(std::make_shared<Triangle>(v4, v0, v3, mat, true, true));
    //     scene.Add(std::make_shared<Triangle>(v4, v3, v7, mat, true, true));

    //     // top
    //     scene.Add(std::make_shared<Triangle>(v3, v2, v6, mat, true, true));
    //     scene.Add(std::make_shared<Triangle>(v3, v6, v7, mat, true, true));

    //     // bottom
    //     scene.Add(std::make_shared<Triangle>(v1, v0, v4, mat, true, true));
    //     scene.Add(std::make_shared<Triangle>(v1, v4, v5, mat, true, true));
    // }

    // Right sphere
    {
        auto mat = std::make_shared<Dielectric>(1.5);
        auto sphere = std::make_shared<Sphere>(Vec3(0.65, 0.15, -0.3), 0.15, mat);
        scene.Add(sphere);

        lights->Add(sphere);
    }

    return lights;
}

Color ComputeRayColor(
    const Ray& ray, const Hittable& scene, std::shared_ptr<Hittable>& lights, const Color& sky_color, int32 depth)
{
    if (depth <= 0)
    {
        return Color{ 0.0 };
    }

    HitRecord rec;
    if (scene.Hit(ray, ray_tolerance, infinity, rec) == false)
    {
        return sky_color;
    }

    Color emitted = rec.mat->Emit(ray, rec);

    ScatterRecord srec;
    if (rec.mat->Scatter(ray, rec, srec) == false)
    {
        return emitted;
    }

    if (srec.is_specular == true)
    {
        return srec.attenuation * ComputeRayColor(srec.specular_ray, scene, lights, sky_color, depth - 1);
    }

#if IMPORTANCE_SAMPLING
    HittablePDF light_pdf{ lights, rec.p };
    MixturePDF mixed_pdf{ &light_pdf, srec.pdf.get() };

    Ray scattered{ rec.p, mixed_pdf.Generate() };
    double pdf_value = mixed_pdf.Evaluate(scattered.dir);

    return emitted + srec.attenuation * rec.mat->ScatteringPDF(ray, rec, scattered) *
                         ComputeRayColor(scattered, scene, lights, sky_color, depth - 1) / pdf_value;
#else
    Ray scattered{ rec.p, srec.pdf->Generate() };
    double pdf_value = srec.pdf->Evaluate(scattered.dir);

    return emitted + srec.attenuation * rec.mat->ScatteringPDF(ray, rec, scattered) *
                         ComputeRayColor(scattered, scene, lights, sky_color, depth - 1) / pdf_value;
#endif
}

int main()
{
#if defined(_WIN32) && defined(_DEBUG)
    // Enable memory-leak reports
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    constexpr double aspect_ratio = 1.0;
    constexpr int32 width = 500;
    constexpr int32 height = static_cast<int32>(width / aspect_ratio);
    constexpr int32 samples_per_pixel = 100;
    constexpr double scale = 1.0 / samples_per_pixel;
    const int max_depth = 10;

    Bitmap bitmap{ width, height };
    Scene scene;

    // Color sky_color{ 0.7, 0.8, 1.0 };
    // TestScene(scene);

    // Vec3 lookfrom(0, 1, 1);
    // Vec3 lookat(0, 0.5, 0);
    // Vec3 vup(0, 1, 0);
    // auto dist_to_focus = (lookfrom - lookat).Length();
    // auto aperture = 0.0;
    // double vFov = 71;

    // Camera camera{ lookfrom, lookat, vup, vFov, aspect_ratio, aperture, dist_to_focus };

    // Color sky_color{ 0.7, 0.8, 1.0 };
    Color sky_color{ 0.0, 0.0, 0.0 };
    std::shared_ptr<Hittable> lights = CornellBox2(scene);

    Vec3 lookfrom{ 0.5, 0.5, 1.25 };
    Vec3 lookat{ 0.5, 0.5, 0.0 };
    Vec3 vup{ 0.0, 1.0, 0.0 };
    auto dist_to_focus = (lookfrom - lookat).Length();
    auto aperture = 0.0;
    double vFov = 45.0;

    Camera camera{ lookfrom, lookat, vup, vFov, aspect_ratio, aperture, dist_to_focus };

    // Color sky_color{ 0.2 };
    // Color sky_color = Color{ 0.7, 0.8, 1.0 } * 0.5;
    // TriangleTest(scene);

    // Vec3 lookfrom(0, 1, 1);
    // Vec3 lookat(0, 0.5, 0);
    // Vec3 vup(0, 1, 0);
    // auto dist_to_focus = (lookfrom - lookat).Length();
    // auto aperture = 0.0;
    // double vFov = 71;

    // Camera camera{ lookfrom, lookat, vup, vFov, aspect_ratio, aperture, dist_to_focus };

    // Color sky_color{ 0.7, 0.8, 1.0 };
    // BVHTest(scene);

    // Vec3 lookfrom(0, 0, 5);
    // Vec3 lookat(0, 0, 0);
    // Vec3 vup(0, 1, 0);
    // auto dist_to_focus = (lookfrom - lookat).Length();
    // auto aperture = 0.0;
    // double vFov = 71;

    // Camera camera{ lookfrom, lookat, vup, vFov, aspect_ratio, aperture, dist_to_focus };

    // Color sky_color{ 0.7, 0.8, 1.0 };
    // sky_color *= 8.0;

    // Sponza(scene);

    // Vec3 lookfrom(0.0, 1.0, 4.5);
    // Vec3 lookat(0.0, 1.45, 0.0);
    // Vec3 vup(0, 1, 0);
    // auto dist_to_focus = (lookfrom - lookat).Length();
    // auto aperture = 0.0;
    // double vFov = 71;

    // Camera camera(lookfrom, lookat, vup, vFov, aspect_ratio, aperture, dist_to_focus);

    auto t0 = std::chrono::system_clock::now();

    // #pragma omp parallel for schedule(dynamic, 1)
    for (int32 y = 0; y < height; ++y)
    {
        // std::cout << "\rScanlines remaining: " << y << ' ' << std::flush;
        if (omp_get_thread_num() == 0)
        {
            std::printf("\rScanline: %d / %d", y, height);
        }

#pragma omp parallel for schedule(dynamic, 1)
        for (int32 x = 0; x < width; ++x)
        {
            Color samples{ 0.0 };

            for (int s = 0; s < samples_per_pixel; ++s)
            {
                double u = (x + Rand()) / (width - 1);
                double v = (y + Rand()) / (height - 1);

                Ray r = camera.GetRay(u, v);
                samples += ComputeRayColor(r, scene, lights, sky_color, max_depth);
            }

            // Divide the color by the number of samples and gamma-correct for gamma=2.2
            double gamma = 1.0 / 2.2;
            Color color = Color{ pow(samples.x * scale, gamma), pow(samples.y * scale, gamma), pow(samples.z * scale, gamma) };
            bitmap.Set(x, y, color);
        }
    }

    auto t1 = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(t1);
    std::chrono::duration<double> d = t1 - t0;

    std::cout << "\nDone!: " << d.count() << 's' << std::endl;

    std::string fileName = "render_" + std::to_string(width) + "x" + std::to_string(height) + "_s" +
                           std::to_string(samples_per_pixel) + "_d" + std::to_string(max_depth) + "_t" +
                           std::to_string(d.count()) + "s.png";

    bitmap.WriteToFile(fileName.c_str());

    return system(fileName.c_str());
}