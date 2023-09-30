#if defined(_WIN32) && defined(_DEBUG)
#include <crtdbg.h>
#endif

#include "spt/spt.h"

#include <format>
#include <omp.h>

// Test scenes
namespace spt
{

extern void RandomScene(Scene&);
extern void BVHTest(Scene&);
extern void CornellBox(Scene&);
extern void Sponza(Scene&);
extern void NormalMapping(Scene&);
extern void PBRTest(Scene&);
extern void EnvironmentMap(Scene&);
extern void BRDFSamplingTest(Scene&);
extern void MISTest1(Scene&);
extern void MISTest2(Scene&);
extern void MISTestWak(Scene&);
extern void GGXVNDFSamplingTest(Scene&);
extern void CornellBoxLucy(Scene&);
extern void CameraScene(Scene&);
extern void StanfordScene(Scene&);
extern void StatueScene(Scene&);
extern void ShipScene(Scene&);
extern void CornellBoxBunnyVolume(Scene&);
extern void CornellBoxOriginal(Scene&);
extern void RebootScene(Scene&);
extern void CornellBoxGlossy(Scene&);
extern void BreakfastRoom(Scene&);

} // namespace spt

int main()
{
#if defined(_WIN32) && defined(_DEBUG)
    // Enable memory-leak reports
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    using namespace spt;

    // Float aspect_ratio = 16.0 / 9.0;
    // Float aspect_ratio = 3.0 / 2.0;
    // Float aspect_ratio = 4.0 / 3.0;
    Float aspect_ratio = 1.0;
    int32 width = 500;
    int32 height = static_cast<int32>(width / aspect_ratio);
    int32 samples_per_pixel = 64;
    Float scale = 1.0 / samples_per_pixel;
    int32 max_bounces = 20;
    Bitmap bitmap{ width, height };

    Scene scene;
    Camera camera;

    Timer timer;

    switch (2)
    {
    case 0: // Raytracing in one weekend final scene
    {
        RandomScene(scene);

        Point3 lookfrom{ 13, 2, 3 };
        Point3 lookat{ 0, 0, 0 };

        Float dist_to_focus = 10.0;
        Float aperture = 0.1;
        Float vFov = 20;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 1: // BVH test
    {
        BVHTest(scene);

        Point3 lookfrom{ 0, 0, 5 };
        Point3 lookat{ 0, 0, 0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 71;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 2: // Cornell box
    {
        CornellBox(scene);

        Point3 lookfrom{ 0.5, 0.5, 1.25 };
        Point3 lookat{ 0.5, 0.5, 0.0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 45.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 3: // Sponza
    {
        Sponza(scene);

        // Point3 lookfrom{ 0.0, 2.5, 4.5 };
        // Point3 lookat{ 0.0, 1.45, 0.0 };

        // Point3 lookfrom{ -1.5, 5.5, 10.0 };
        // Point3 lookat{ 0.0, 3.45, 0.0 };

        // Point3 lookfrom{ 0.0, 0.5, 7.0 };
        // Point3 lookat{ 0.0, 3.0, 0.0 };

        Point3 lookfrom{ 0.0, 5.0, 6.0 };
        Point3 lookat{ 0.0, 5.0, 0.0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 71;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 4: // Normal mapping test
    {
        NormalMapping(scene);

        // Point3 lookfrom{ 10.0, 0.0, 10.0 };
        // Point3 lookat{ 3.0, -2.5, 1.0 };

        Point3 lookfrom = Point3(1.0, 0.5, 4.0) * 1.2;
        Point3 lookat = Point3(0.0, 0.0, 0.0);

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 30.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 5: // PBR test
    {
        PBRTest(scene);

        Point3 lookfrom{ 0.0, 4.0, 5.0 };
        Point3 lookat{ 0.0, 0.0, 0.0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 71.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 6: // Environment map test
    {
        EnvironmentMap(scene);

        Point3 lookfrom{ 0.0, 3.0, 5.0 };
        Point3 lookat{ 0.0, 0.0, 0.0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 71.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 7: // BRDF sampling test
    {
        BRDFSamplingTest(scene);

        Point3 lookfrom{ 0.5, 0.5, 1.25 };
        Point3 lookat{ 0.5, 0.5, 0.0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 45.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 8: // MIS test
    {
        MISTest1(scene);

        Float y = 0.345832;

        Point3 lookfrom{ 0.0, y, 1.0 };
        Point3 lookat{ 0.0, y, 0.0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 45.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 9: // MIS test (original)
    {
        MISTest2(scene);

        Point3 lookfrom{ 0.0, 2, 15 };
        Point3 lookat{ 0.0, -2, 2.5 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 28.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 10: // MIS test (wakgood)
    {
        MISTestWak(scene);

        Point3 lookfrom{ 0.0, 2, 15 };
        Point3 lookat{ 0.0, -2, 2.5 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 28.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 11: // GGXVNDF sampling test
    {
        GGXVNDFSamplingTest(scene);

        Point3 lookfrom{ 0.0, 2.0, 10.0 };
        Point3 lookat{ 0.0, 1.0, 0.0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 30.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 12: // Lucy
    {
        CornellBoxLucy(scene);

        Point3 lookfrom{ 0.5, 0.5, 1.25 };
        Point3 lookat{ 0.5, 0.5, 0.0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 45.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 13: // Antique camera
    {
        CameraScene(scene);

        Point3 lookfrom{ -2.0, 1.0, 2.0 };
        Point3 lookat{ 0.0, 0.5, 0.0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 30.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 14: // Stanford models
    {
        StanfordScene(scene);

        Point3 lookfrom{ 0.0, 0.5, 2.0 };
        Point3 lookat{ 0.0, 0.2, 0.0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 30.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 15: // Statue scene
    {
        StatueScene(scene);

        Point3 lookfrom{ 0.0, 0.0, 10.0 };
        Point3 lookat{ 0.0, 0.0, 0.0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 30.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 16: // Ship
    {
        ShipScene(scene);

        Point3 lookfrom{ 5.0, 5.0, 10.0 };
        Point3 lookat{ 0.0, 2.8, 0.0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 30.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 17: // Constant volume
    {
        CornellBoxBunnyVolume(scene);

        Point3 lookfrom{ 0.5, 0.5, 1.25 };
        Point3 lookat{ 0.5, 0.5, 0.0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 45.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 18: // Original cornell box scene
    {
        CornellBoxOriginal(scene);

        Point3 lookfrom{ 0.5, 0.5, 1.64 };
        Point3 lookat{ 0.5, 0.5, 0.0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 35.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 19: // Reboot robot scene
    {
        RebootScene(scene);

        Point3 lookfrom{ -4.0, 3.5, -4.0 };
        Point3 lookat{ 0.0, 0.0, 0.0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.02;
        Float vFov = 30.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 20: // Glossy cornell box
    {
        CornellBoxGlossy(scene);

        Point3 lookfrom{ 0.5, 0.5, 1.25 };
        Point3 lookat{ 0.5, 0.5, 0.0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 45.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 21: // Breakfast room
    {
        BreakfastRoom(scene);

        Point3 lookfrom{ 0.0, 2.2, 4.5 };
        Point3 lookat{ 0.0, 1.5, 0.0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0;
        Float vFov = 71.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    default:
        assert(false);
        break;
    }

    timer.Mark();
    Float t = timer.Get();
    std::cout << "Scene construction: " << t << "s" << std::endl;

    // #pragma omp parallel for schedule(dynamic, 1)
    for (int32 y = 0; y < height; ++y)
    {
        if (omp_get_thread_num() == 0)
        {
            std::printf("\rScanline: %d / %d", y, height);
        }

#pragma omp parallel for schedule(dynamic, 1)
        for (int32 x = 0; x < width; ++x)
        {
            Color samples{ 0.0, 0.0, 0.0 };

            for (size_t s = 0; s < samples_per_pixel; ++s)
            {
                Float u = (x + Rand()) / (width - 1);
                Float v = (y + Rand()) / (height - 1);

                Ray ray = camera.GetRay(u, v);
                samples += PathTrace(scene, ray, max_bounces);
            }

            if (IsNullish(samples))
            {
                std::cout << "null" << std::endl;
            }

            // Resolve NaNs
            if (samples.x != samples.x) samples.x = 0.0;
            if (samples.y != samples.y) samples.y = 0.0;
            if (samples.z != samples.z) samples.z = 0.0;

            Color color = samples * scale;
            color = Tonemap_ACES(color);
            color = GammaCorrection(color, 2.2);

            bitmap.Set(x, y, color);
        }
    }

    timer.Mark();
    t = timer.Get();

    std::cout << "\nDone!: " << t << 's' << std::endl;

    std::string fileName = std::format("render_{}x{}_s{}_d{}_t{}s.png", width, height, samples_per_pixel, max_bounces, t);

    bitmap.WriteToFile(fileName.c_str());

#if _DEBUG
    return 0;
#else
    return system(fileName.c_str());
#endif
}