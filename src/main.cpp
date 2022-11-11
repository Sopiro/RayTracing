#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "raytracer/bitmap.h"
#include "raytracer/camera.h"
#include "raytracer/common.h"
#include "raytracer/hittable_list.h"
#include "raytracer/material.h"
#include "raytracer/ray.h"
#include "raytracer/sphere.h"

#include <omp.h>

HittableList RandomScene()
{
    HittableList world;

    auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(Vec3(0, -1000, 0), 1000, ground_material));

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
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = Color::Random(0.5, 1);
                    auto fuzz = Rand(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(Vec3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(std::make_shared<Sphere>(Vec3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(Vec3(4, 1, 0), 1.0, material3));

    return world;
}

HittableList TestScene()
{
    HittableList world;

    auto material_ground = std::make_shared<Lambertian>(Color{ 0.8, 0.8, 0.8 });
    auto material_center = std::make_shared<Lambertian>(Color{ 0.1, 0.2, 0.5 });
    auto material_left = std::make_shared<Dielectric>(1.5);
    auto material_right = std::make_shared<Metal>(Color{ 0.8, 0.6, 0.2 }, 0.0);
    auto checkerTexture = std::make_shared<CheckerTexture>(Color{ 0.2, 0.3, 0.1 }, Color{ 0.9, 0.9, 0.9 });
    auto checker = std::make_shared<Lambertian>(checkerTexture);

    auto light = std::make_shared<DiffuseLight>(Vec3{ 3.0 });

    world.add(std::make_shared<Sphere>(Vec3{ 0.0, -100.5, -1.0 }, 100.0, checker));
    world.add(std::make_shared<Sphere>(Vec3{ 0.0, 0.0, -1.0 }, 0.5, material_center));
    world.add(std::make_shared<Sphere>(Vec3{ -1.0, 0.0, -1.0 }, 0.5, material_left));
    world.add(std::make_shared<Sphere>(Vec3(-1.0, 0.0, -1.0), -0.45, material_left));
    world.add(std::make_shared<Sphere>(Vec3{ 1.0, 0.0, -1.0 }, 0.5, material_right));

    world.add(std::make_shared<Sphere>(Vec3{ 0.0, 2.0, 0.0 }, 0.5, light));
    world.add(std::make_shared<Sphere>(Vec3{ 5.0, 2.0, -5.0 }, 0.5, light));
    world.add(std::make_shared<Sphere>(Vec3{ -5.0, 2.0, -5.0 }, 0.5, light));

    return world;
}

HittableList CornellBox()
{
    HittableList objects;

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

    objects.add(std::make_shared<Sphere>(Vec3{ -r, m, m }, r, green));        // left
    objects.add(std::make_shared<Sphere>(Vec3{ r + g, m, m }, r, red));       // right
    objects.add(std::make_shared<Sphere>(Vec3{ m, m, -r }, r, white));        // front
    objects.add(std::make_shared<Sphere>(Vec3{ m, m, r + 2.41 }, r, absorb)); // back
    objects.add(std::make_shared<Sphere>(Vec3{ m, -r, m }, r, white));        // bottom
    objects.add(std::make_shared<Sphere>(Vec3{ m, r + g, m }, r, white));     // top

    objects.add(std::make_shared<Sphere>(Vec3{ m, 10.0, m }, 9.003, light)); // light

    objects.add(std::make_shared<Sphere>(Vec3{ 0.8, 0.13, 0.5 }, 0.13, glass));
    objects.add(std::make_shared<Sphere>(Vec3{ 0.3, 0.18, 0.8 }, 0.18, metal));
    // objects.add(std::make_shared<Sphere>(Vec3{ 0.3, 0.2, 0.7 }, -0.19, glass));

    return objects;
}

Color ComputeRayColor(const Ray& r, const Hittable& world, const Color& sky_color, int32 depth)
{
    if (depth <= 0)
    {
        return Color{ 0.0f };
    }

    HitRecord rec;
    if (world.Hit(r, 0.00001, infinity, rec) == false)
    {
        return sky_color;
    }

    Ray scattered;
    Color attenuation;
    Color emitted = rec.mat->Emitted(rec.uv, rec.p);

    if (rec.mat->Scatter(r, rec, attenuation, scattered) == false)
    {
        return emitted;
    }

    return emitted + attenuation * ComputeRayColor(scattered, world, sky_color, depth - 1);

    // Vec3 unit_direction = r.dir.Normalized();
    // double t = 0.5 * (unit_direction.y + 1.0);

    // return Lerp(Color{ 1.0, 1.0, 1.0 }, Color{ 0.5, 0.7, 1.0 }, t);
}

int main()
{
    constexpr double aspect_ratio = 16.0 / 9.0;
    constexpr int32 width = 640;
    constexpr int32 height = static_cast<int32>(width / aspect_ratio);
    constexpr int32 samples_per_pixel = 100;
    constexpr double scale = 1.0 / samples_per_pixel;
    const int max_depth = 50;

    Bitmap bitmap{ width, height };

    Color sky_color{ 0.7, 0.8, 1.0 };
    HittableList world = TestScene();

    Vec3 lookfrom(0, 1, 1);
    Vec3 lookat(0, 0.5, 0);
    Vec3 vup(0, 1, 0);
    auto dist_to_focus = (lookfrom - lookat).Length();
    auto aperture = 0.0;
    double vFov = 71;

    Camera camera{ lookfrom, lookat, vup, vFov, aspect_ratio, aperture, dist_to_focus };

    // HittableList world = CornellBox();

    // Vec3 lookfrom(0.5, 0.5, 2.4);
    // Vec3 lookat(0.5, 0.5, 0.0);
    // Vec3 vup(0, 1, 0);
    // auto dist_to_focus = (lookfrom - lookat).Length();
    // auto aperture = 0.0;
    // double vFov = 40;

    // Camera camera(lookfrom, lookat, vup, vFov, aspect_ratio, aperture, dist_to_focus);

    auto t0 = std::chrono::system_clock::now();

    double chunk = height / omp_get_max_threads();

#pragma omp parallel for
    for (int32 y = 0; y < height; ++y)
    {
        // std::cout << "\rScanlines remaining: " << y << ' ' << std::flush;
        if (omp_get_thread_num() == 0)
        {
            std::printf("\rProcessing... %.2lf%%", double(y) / (chunk - 1) * 100.0);
        }

        for (int32 x = 0; x < width; ++x)
        {
            Color samples{ 0.0 };

            for (int s = 0; s < samples_per_pixel; ++s)
            {
                double u = (x + Rand()) / (width - 1);
                double v = (y + Rand()) / (height - 1);

                Ray r = camera.GetRay(u, v);
                samples += ComputeRayColor(r, world, sky_color, max_depth);
            }

            // Divide the color by the number of samples and gamma-correct for gamma=2.2.
            Vec3 color =
                Vec3{ pow(samples.x * scale, 1.0 / 2.2), pow(samples.y * scale, 1.0 / 2.2), pow(samples.z * scale, 1.0 / 2.2) };
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