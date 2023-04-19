#if defined(_WIN32) && defined(_DEBUG)
#include <crtdbg.h>
#endif

#include "spt/pathtracer.h"

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

} // namespace spt

using namespace spt;

int main()
{
#if defined(_WIN32) && defined(_DEBUG)
    // Enable memory-leak reports
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    // double aspect_ratio = 16.0 / 9.0;
    // double aspect_ratio = 3.0 / 2.0;
    double aspect_ratio = 4.0 / 3.0;
    // double aspect_ratio = 1.0;
    int32 width = 640;
    int32 height = static_cast<int32>(width / aspect_ratio);
    int32 samples_per_pixel = 64;
    double scale = 1.0 / samples_per_pixel;
    // int32 bounce_count = 10;
    int32 bounce_count = INT_MAX;
    Bitmap bitmap{ width, height };

    Scene scene;
    Camera camera;

    switch (11)
    {
    case 0: // Raytracing in one weekend final scene
    {
        RandomScene(scene);

        Point3 lookfrom{ 13, 2, 3 };
        Point3 lookat{ 0, 0, 0 };

        auto dist_to_focus = 10.0;
        auto aperture = 0.1;
        double vFov = 20;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 1: // BVH test
    {
        BVHTest(scene);

        Point3 lookfrom{ 0, 0, 5 };
        Point3 lookat{ 0, 0, 0 };

        auto dist_to_focus = (lookfrom - lookat).Length();
        auto aperture = 0.0;
        double vFov = 71;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 2: // Cornell box
    {
        CornellBox(scene);

        Point3 lookfrom{ 0.5, 0.5, 1.25 };
        Point3 lookat{ 0.5, 0.5, 0.0 };

        auto dist_to_focus = (lookfrom - lookat).Length();
        auto aperture = 0.0;
        double vFov = 45.0;

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

        auto dist_to_focus = (lookfrom - lookat).Length();
        auto aperture = 0.0;
        double vFov = 71;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 4: // Normal mapping test
    {
        NormalMapping(scene);

        // Point3 lookfrom{ 10.0, 0.0, 10.0 };
        // Point3 lookat{ 3.0, -2.5, 1.0 };

        Point3 lookfrom = Point3{ 1.0, 0.5, 4.0 } * 1.2;
        Point3 lookat{ 0.0, 0.0, 0.0 };

        auto dist_to_focus = (lookfrom - lookat).Length();
        auto aperture = 0.0;
        double vFov = 30.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 5: // PBR test
    {
        PBRTest(scene);

        Point3 lookfrom{ 0.0, 4.0, 5.0 };
        Point3 lookat{ 0.0, 0.0, 0.0 };

        auto dist_to_focus = (lookfrom - lookat).Length();
        auto aperture = 0.0;
        double vFov = 71.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 6: // Environment map test
    {
        EnvironmentMap(scene);

        Point3 lookfrom{ 0.0, 3.0, 5.0 };
        Point3 lookat{ 0.0, 0.0, 0.0 };

        auto dist_to_focus = (lookfrom - lookat).Length();
        auto aperture = 0.0;
        double vFov = 71.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 7: // BRDF sampling test
    {
        BRDFSamplingTest(scene);

        Point3 lookfrom{ 0.5, 0.5, 1.25 };
        Point3 lookat{ 0.5, 0.5, 0.0 };

        auto dist_to_focus = (lookfrom - lookat).Length();
        auto aperture = 0.0;
        double vFov = 45.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 8: // MIS test
    {
        MISTest1(scene);

        double y = 0.345832;

        Point3 lookfrom{ 0.0, y, 1.0 };
        Point3 lookat{ 0.0, y, 0.0 };

        auto dist_to_focus = (lookfrom - lookat).Length();
        auto aperture = 0.0;
        double vFov = 45.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 9: // MIS test (original)
    {
        MISTest2(scene);

        Point3 lookfrom{ 0.0, 2, 15 };
        Point3 lookat{ 0.0, -2, 2.5 };

        auto dist_to_focus = (lookfrom - lookat).Length();
        auto aperture = 0.0;
        double vFov = 28.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 10: // MIS test (wakgood)
    {
        MISTestWak(scene);

        Point3 lookfrom{ 0.0, 2, 15 };
        Point3 lookat{ 0.0, -2, 2.5 };

        auto dist_to_focus = (lookfrom - lookat).Length();
        auto aperture = 0.0;
        double vFov = 28.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 11: // GGXVNDF sampling test
    {
        GGXVNDFSamplingTest(scene);

        Point3 lookfrom{ 0.0, 2.0, 10.0 };
        Point3 lookat{ 0.0, 1.0, 0.0 };

        auto dist_to_focus = (lookfrom - lookat).Length();
        auto aperture = 0.0;
        double vFov = 30.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 12: // Lucy
    {
        CornellBoxLucy(scene);

        Point3 lookfrom{ 0.5, 0.5, 1.25 };
        Point3 lookat{ 0.5, 0.5, 0.0 };

        auto dist_to_focus = (lookfrom - lookat).Length();
        auto aperture = 0.0;
        double vFov = 45.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 13: // Antique camera
    {
        CameraScene(scene);

        Point3 lookfrom{ -2.0, 1.0, 2.0 };
        Point3 lookat{ 0.0, 0.5, 0.0 };

        auto dist_to_focus = (lookfrom - lookat).Length();
        auto aperture = 0.0;
        double vFov = 30.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 14: // Stanford models
    {
        StanfordScene(scene);

        Point3 lookfrom{ 0.0, 0.5, 2.0 };
        Point3 lookat{ 0.0, 0.2, 0.0 };

        auto dist_to_focus = (lookfrom - lookat).Length();
        auto aperture = 0.0;
        double vFov = 30.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 15: // Statue scene
    {
        StatueScene(scene);

        Point3 lookfrom{ 0.0, 0.0, 10.0 };
        Point3 lookat{ 0.0, 0.0, 0.0 };

        auto dist_to_focus = (lookfrom - lookat).Length();
        auto aperture = 0.0;
        double vFov = 30.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 16: // Ship
    {
        ShipScene(scene);

        Point3 lookfrom{ 5.0, 5.0, 10.0 };
        Point3 lookat{ 0.0, 2.8, 0.0 };

        auto dist_to_focus = (lookfrom - lookat).Length();
        auto aperture = 0.0;
        double vFov = 30.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    case 17: // volume
    {
        CornellBoxBunnyVolume(scene);

        Point3 lookfrom{ 0.5, 0.5, 1.25 };
        Point3 lookat{ 0.5, 0.5, 0.0 };

        auto dist_to_focus = (lookfrom - lookat).Length();
        auto aperture = 0.0;
        double vFov = 45.0;

        camera = Camera{ lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus };
    }
    break;

    default:
        assert(false);
        break;
    }

    auto t0 = std::chrono::system_clock::now();

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
                double u = (x + Rand()) / (width - 1);
                double v = (y + Rand()) / (height - 1);

                Ray ray = camera.GetRay(u, v);
                samples += PathTrace(scene, ray, bounce_count);
            }

            if (is_nullish(samples))
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

    auto t1 = std::chrono::system_clock::now();
    std::chrono::duration<double> d = t1 - t0;

    std::cout << "\nDone!: " << d.count() << 's' << std::endl;

    std::ostringstream oss;
    oss << "render_" << width << "x" << height << "_s" << samples_per_pixel << "_d" << bounce_count << "_t" << d.count()
        << "s.png";

    const char* fileName = oss.str().c_str();

    bitmap.WriteToFile(fileName);

#if _DEBUG
    return 0;
#else
    return system(fileName);
#endif
}