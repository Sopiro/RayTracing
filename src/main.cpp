#if defined(_WIN32) && defined(_DEBUG)
#include <crtdbg.h>
#endif

#include "raytracer/raytracer.h"

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

    Real r = 1.0e5;
    Real g = 1;
    Real m = g / Real(2.0);

    scene.Add(std::make_shared<Sphere>(Vec3{ -r, m, m }, r, green));              // left
    scene.Add(std::make_shared<Sphere>(Vec3{ r + g, m, m }, r, red));             // right
    scene.Add(std::make_shared<Sphere>(Vec3{ m, m, -r }, r, white));              // front
    scene.Add(std::make_shared<Sphere>(Vec3{ m, m, r + Real(2.41) }, r, absorb)); // back
    scene.Add(std::make_shared<Sphere>(Vec3{ m, -r, m }, r, white));              // bottom
    scene.Add(std::make_shared<Sphere>(Vec3{ m, r + g, m }, r, white));           // top

    scene.Add(std::make_shared<Sphere>(Vec3{ m, 10.0, m }, 9.003, light)); // light

    scene.Add(std::make_shared<Sphere>(Vec3{ 0.8, 0.13, 0.5 }, 0.13, glass));
    scene.Add(std::make_shared<Sphere>(Vec3{ 0.3, 0.18, 0.8 }, 0.18, metal));
    // objects.add(std::make_shared<Sphere>(Vec3{ 0.3, 0.2, 0.7 }, -0.19, glass));
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

    Real p = 5.0;
    Real y = -0.5;
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

    Real n = 30.0;
    Real w = 7.0;
    Real h = w * 9.0 / 16.0;
    Real r = 0.1;

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

Color ComputeRayColor(const Ray& ray, const Hittable& scene, const Color& sky_color, int32 depth)
{
    if (depth <= 0)
    {
        return Color{ 0.0 };
    }

    HitRecord rec;
    if (scene.Hit(ray, 0.00001, infinity, rec) == false)
    {
        return sky_color;
    }

    Ray scattered;
    Color attenuation;
    Color emitted = rec.mat->Emitted(rec.uv, rec.p);

    if (rec.mat->Scatter(ray, rec, attenuation, scattered) == false)
    {
        return emitted;
    }

    return emitted + attenuation * ComputeRayColor(scattered, scene, sky_color, depth - 1);

    // Vec3 unit_direction = r.dir.Normalized();
    // Real t = 0.5 * (unit_direction.y + 1.0);

    // return Lerp(Color{ 1.0, 1.0, 1.0 }, Color{ 0.5, 0.7, 1.0 }, t);
}

int main()
{
#if defined(_WIN32) && defined(_DEBUG)
    // Enable memory-leak reports
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    constexpr Real aspect_ratio = 1.0;
    constexpr int32 width = 500;
    constexpr int32 height = static_cast<int32>(width / aspect_ratio);
    constexpr int32 samples_per_pixel = 100;
    constexpr Real scale = 1.0 / samples_per_pixel;
    const int max_depth = 50;

    Bitmap bitmap{ width, height };
    Scene scene;

    // Color sky_color{ 0.7, 0.8, 1.0 };
    // TestScene(scene);

    // Vec3 lookfrom(0, 1, 1);
    // Vec3 lookat(0, 0.5, 0);
    // Vec3 vup(0, 1, 0);
    // auto dist_to_focus = (lookfrom - lookat).Length();
    // auto aperture = 0.0;
    // Real vFov = 71;

    // Camera camera{ lookfrom, lookat, vup, vFov, aspect_ratio, aperture, dist_to_focus };

    Color sky_color{ 0.7, 0.8, 1.0 };
    CornellBox(scene);

    Vec3 lookfrom(0.5, 0.5, 2.4);
    Vec3 lookat(0.5, 0.5, 0.0);
    Vec3 vup(0, 1, 0);
    auto dist_to_focus = (lookfrom - lookat).Length();
    auto aperture = 0.0;
    Real vFov = 40;

    Camera camera(lookfrom, lookat, vup, vFov, aspect_ratio, aperture, dist_to_focus);

    // Color sky_color{ 0.2 };
    // Color sky_color = Color{ 0.7, 0.8, 1.0 } * 0.5;
    // TriangleTest(scene);

    // Vec3 lookfrom(0, 1, 1);
    // Vec3 lookat(0, 0.5, 0);
    // Vec3 vup(0, 1, 0);
    // auto dist_to_focus = (lookfrom - lookat).Length();
    // auto aperture = 0.0;
    // Real vFov = 71;

    // Camera camera{ lookfrom, lookat, vup, vFov, aspect_ratio, aperture, dist_to_focus };

    // Color sky_color{ 0.7, 0.8, 1.0 };
    // BVHTest(scene);

    // Vec3 lookfrom(0, 0, 5);
    // Vec3 lookat(0, 0, 0);
    // Vec3 vup(0, 1, 0);
    // auto dist_to_focus = (lookfrom - lookat).Length();
    // auto aperture = 0.0;
    // Real vFov = 71;

    // Camera camera{ lookfrom, lookat, vup, vFov, aspect_ratio, aperture, dist_to_focus };

    auto t0 = std::chrono::system_clock::now();

    Real chunk = Real(height) / omp_get_max_threads();

    // Assimp::Importer importer;
    // const aiScene* scene = importer.ReadFile("res/cube.obj", aiProcess_Triangulate | aiProcess_FlipUVs);

    // if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    // {
    //     std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    //     return 0;
    // }

    // std::cout << scene->mMeshes[0]->mNumFaces << std::endl;
    // return 0;

#pragma omp parallel for
    for (int32 y = 0; y < height; ++y)
    {
        // std::cout << "\rScanlines remaining: " << y << ' ' << std::flush;
        if (omp_get_thread_num() == 0)
        {
            std::printf("\rProcessing... %.2lf%%", Real(y) / (chunk - 1) * 100.0);
        }

        for (int32 x = 0; x < width; ++x)
        {
            Color samples{ 0.0 };

            for (int s = 0; s < samples_per_pixel; ++s)
            {
                Real u = (x + Rand()) / (width - 1);
                Real v = (y + Rand()) / (height - 1);

                Ray r = camera.GetRay(u, v);
                samples += ComputeRayColor(r, scene, sky_color, max_depth);
            }

            // Divide the color by the number of samples and gamma-correct for gamma=2.2
            Real gamma = 1.0 / 2.2;
            Color color = Color{ pow(samples.x * scale, gamma), pow(samples.y * scale, gamma), pow(samples.z * scale, gamma) };
            bitmap.Set(x, y, color);
        }
    }

    auto t1 = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(t1);
    std::chrono::duration<Real> d = t1 - t0;

    std::cout << "\nDone!: " << d.count() << 's' << std::endl;

    std::string fileName = "render_" + std::to_string(width) + "x" + std::to_string(height) + "_s" +
                           std::to_string(samples_per_pixel) + "_d" + std::to_string(max_depth) + "_t" +
                           std::to_string(d.count()) + "s.png";

    bitmap.WriteToFile(fileName.c_str());

    return system(fileName.c_str());
}