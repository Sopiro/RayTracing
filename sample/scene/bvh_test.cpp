#include "../samples.h"
#include "bulbit/diffuse_light.h"
#include "bulbit/lambertian.h"
#include "bulbit/perspective_camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

std::unique_ptr<Camera> BVHTest(Scene& scene)
{
    auto gray = std::make_shared<Lambertian>(Spectrum(0.8f, 0.8f, 0.8f));
    auto red = std::make_shared<Lambertian>(Spectrum(.65f, .05f, .05f));
    auto green = std::make_shared<Lambertian>(Spectrum(.12f, .45f, .15f));
    auto blue = std::make_shared<Lambertian>(Spectrum(.22f, .23f, .75f));
    auto white = std::make_shared<Lambertian>(Spectrum(.73f, .73f, .73f));
    auto black = std::make_shared<Lambertian>(Spectrum(0.0f));

    Float n = 100.0f;
    Float w = 7.0f;
    Float h = w * 9.0f / 16.0f;
    Float r = 0.05f;

    for (int32 y = 0; y < n; ++y)
    {
        for (int32 x = 0; x < n; ++x)
        {
            Vec3 pos;
            // pos.x = x / n * w - w / 2.0;
            // pos.y = y / n * w - w / 2.0;

            pos.x = Rand(-w, w);
            pos.y = Rand(-h, h);
            pos.z = -1;

            scene.Add(std::make_shared<Sphere>(pos, r, green));
        }
    }

    scene.AddLight(std::make_shared<InfiniteAreaLight>("res/HDR/quarry_04_puresky_1k.hdr"));

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0, 0, 5 };
    Point3 lookat{ 0, 0, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0.0f;
    Float vFov = 71.f;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("bvh", BVHTest);

} // namespace bulbit