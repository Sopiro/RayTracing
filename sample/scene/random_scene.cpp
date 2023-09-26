#include "spt/spt.h"

namespace spt
{

// The final scene of Ray Tracing in One Weekend
// https://raytracing.github.io/books/RayTracingInOneWeekend.html#wherenext?/afinalrender
void RandomScene(Scene& scene)
{
    auto ground_material = CreateSharedRef<Lambertian>(Color(0.5, 0.5, 0.5));
    scene.Add(CreateSharedRef<Sphere>(Vec3(0, -1000, 0), 1000, ground_material));

    for (i32 a = -11; a < 11; a++)
    {
        for (i32 b = -11; b < 11; b++)
        {
            auto choose_mat = Rand();
            Vec3 center(a + 0.9 * Rand(), 0.2, b + 0.9 * Rand());

            if ((center - Vec3(4, 0.2, 0)).Length() > 0.9)
            {
                Ref<Material> sphere_material;

                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto albedo = RandVec3() * RandVec3();
                    sphere_material = CreateSharedRef<Lambertian>(albedo);
                    scene.Add(CreateSharedRef<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = RandVec3(0.5, 1.0);
                    auto fuzz = Rand(0.0, 0.5);
                    sphere_material = CreateSharedRef<Metal>(albedo, fuzz);
                    scene.Add(CreateSharedRef<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = CreateSharedRef<Dielectric>(1.5);
                    scene.Add(CreateSharedRef<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = CreateSharedRef<Dielectric>(1.5);
    scene.Add(CreateSharedRef<Sphere>(Vec3(0, 1, 0), 1.0, material1));

    auto material2 = CreateSharedRef<Lambertian>(Color(0.4, 0.2, 0.1));
    scene.Add(CreateSharedRef<Sphere>(Vec3(-4, 1, 0), 1.0, material2));

    auto material3 = CreateSharedRef<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    scene.Add(CreateSharedRef<Sphere>(Vec3(4, 1, 0), 1.0, material3));

    scene.SetEnvironmentMap(ImageTexture::Create("res/sunflowers/sunflowers_puresky_4k.hdr", false, true));
}

} // namespace spt
