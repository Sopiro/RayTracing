#if defined(_WIN32) && defined(_DEBUG)
#include <crtdbg.h>
#endif

#include "bulbit/bulbit.h"

#include <format>
#include <omp.h>

// Test scenes
namespace bulbit
{

extern void RandomScene(Scene&);
extern void BVHTest(Scene&);
extern void CornellBox(Scene&);
extern void Sponza(Scene&);
extern void NormalMapping(Scene&);
extern void PBRTest(Scene&);
extern void BRDFSamplingTest(Scene&);
extern void MISTest(Scene&);
extern void MISTestWak(Scene&);
extern void GGXVNDFSamplingTest(Scene&);
extern void CornellBoxLucy(Scene&);
extern void CameraScene(Scene&);
extern void StanfordScene(Scene&);
extern void StatueScene(Scene&);
extern void ShipScene(Scene&);
extern void CornellBoxBunnyVolume(Scene&);
extern void RebootScene(Scene&);
extern void CornellBoxGlossy(Scene&);
extern void BreakfastRoom(Scene&);
extern void BistroScene(Scene&);
extern void SunTempleScene(Scene&);

} // namespace bulbit

int main()
{
#if defined(_WIN32) && defined(_DEBUG)
    // Enable memory-leak reports
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    using namespace bulbit;

    // Float aspect_ratio = 16.0f / 9.0f;
    // Float aspect_ratio = 3.0f / 2.0f;
    // Float aspect_ratio = 4.0f / 3.0f;
    Float aspect_ratio = 1.0f;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);
    int32 samples_per_pixel = 64;
    int32 max_bounces = 50;

    Scene scene;
    std::unique_ptr<Camera> camera;

    Timer timer;

    switch (2)
    {
    case 0: // Raytracing in one weekend final scene
    {
        RandomScene(scene);

        Point3 lookfrom{ 13, 2, 3 };
        Point3 lookat{ 0, 0, 0 };

        Float dist_to_focus = 10.0f;
        Float aperture = 0.1f;
        Float vFov = 20;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 1: // BVH test
    {
        BVHTest(scene);

        Point3 lookfrom{ 0, 0, 5 };
        Point3 lookat{ 0, 0, 0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0f;
        Float vFov = 71.f;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 2: // Cornell box
    {
        CornellBox(scene);

        Point3 lookfrom{ 0.5f, 0.5f, 1.64f };
        Point3 lookat{ 0.5f, 0.5f, 0.0f };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.0f;
        Float vFov = 35.0f;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 3: // Sponza
    {
        Sponza(scene);

        // Point3 lookfrom{ 0.0f, 2.5f, 4.5f };
        // Point3 lookat{ 0.0f, 1.45f, 0.0f };

        // Point3 lookfrom{ -1.5f, 5.f5, 10.0f };
        // Point3 lookat{ 0.0f, 3.45f, 0.0f };

        // Point3 lookfrom{ 0.0f, 0.5f, 7.0f };
        // Point3 lookat{ 0.0f, 3.0f, 0.0f };

        Point3 lookfrom{ 0, 5, 6 };
        Point3 lookat{ 0, 5, 0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0;
        Float vFov = 71;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 4: // Normal mapping test
    {
        NormalMapping(scene);

        // Point3 lookfrom{ 10.0, 0.0, 10.0 };
        // Point3 lookat{ 3.0, -2.5, 1.0 };

        Point3 lookfrom = Point3(1, 0.5f, 4) * 1.2f;
        Point3 lookat = Point3(0, 0, 0);

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0;
        Float vFov = 30;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 5: // PBR test
    {
        PBRTest(scene);

        Point3 lookfrom{ 0, 4, 5 };
        Point3 lookat{ 0, 0, 0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0;
        Float vFov = 71;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 6: // BRDF sampling test
    {
        BRDFSamplingTest(scene);

        Point3 lookfrom{ 0.5f, 0.5f, 1.25f };
        Point3 lookat{ 0.5f, 0.5f, 0.0f };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0;
        Float vFov = 45;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 7: // MIS test (original)
    {
        MISTest(scene);

        Point3 lookfrom{ 0, 2, 15 };
        Point3 lookat{ 0, -2, 2.5f };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0;
        Float vFov = 28;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 8: // MIS test (wakgood)
    {
        MISTestWak(scene);

        Point3 lookfrom{ 0, 2, 15 };
        Point3 lookat{ 0, -2, 2.5f };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0;
        Float vFov = 28;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 9: // GGXVNDF sampling test
    {
        GGXVNDFSamplingTest(scene);

        Point3 lookfrom{ 0, 2, 10 };
        Point3 lookat{ 0, 1, 0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0;
        Float vFov = 30;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 10: // Lucy
    {
        CornellBoxLucy(scene);

        Point3 lookfrom{ 0.5f, 0.5f, 1.25f };
        Point3 lookat{ 0.5f, 0.5f, 0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0;
        Float vFov = 45;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 11: // Antique camera
    {
        CameraScene(scene);

        Point3 lookfrom{ -2, 1, 2 };
        Point3 lookat{ 0, 0.5f, 0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0;
        Float vFov = 30;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 12: // Stanford models
    {
        StanfordScene(scene);

        Point3 lookfrom{ 0, 0.5f, 2 };
        Point3 lookat{ 0, 0.2f, 0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0;
        Float vFov = 30;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 13: // Statue scene
    {
        StatueScene(scene);

        Point3 lookfrom{ 0, 0, 10 };
        Point3 lookat{ 0, 0, 0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0;
        Float vFov = 30;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 14: // Ship
    {
        ShipScene(scene);

        Point3 lookfrom{ 5, 5, 10 };
        Point3 lookat{ 0, 2.8f, 0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0;
        Float vFov = 30;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 15: // Constant volume
    {
        CornellBoxBunnyVolume(scene);

        Point3 lookfrom{ 0.5f, 0.5f, 1.25f };
        Point3 lookat{ 0.5f, 0.5f, 0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0;
        Float vFov = 45;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 16: // Reboot robot scene
    {
        RebootScene(scene);

        Point3 lookfrom{ -4, 3.5f, -4 };
        Point3 lookat{ 0, 0, 0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0.02f;
        Float vFov = 30;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 17: // Glossy cornell box
    {
        CornellBoxGlossy(scene);

        Point3 lookfrom{ 0.5f, 0.5f, 1.25f };
        Point3 lookat{ 0.5f, 0.5f, 0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0;
        Float vFov = 45;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 18: // Breakfast room
    {
        BreakfastRoom(scene);

        Point3 lookfrom{ 0, 2.2f, 4.5f };
        Point3 lookat{ 0, 1.5f, 0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0;
        Float vFov = 71;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 19: // Bistro scene
    {
        BistroScene(scene);

        Point3 lookfrom{ -21, 6, 0 };
        Point3 lookat{ 0, 1, 0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0;
        Float vFov = 54;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    case 20: // Sun temple scene
    {
        SunTempleScene(scene);

        Point3 lookfrom{ -4.48045f, 9.22976f, -7.49469f };
        Point3 lookat{ 0, 8, 0 };

        Float dist_to_focus = (lookfrom - lookat).Length();
        Float aperture = 0;
        Float vFov = 54;

        camera = std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
    }
    break;

    default:
        assert(false);
        break;
    }

    Ref<Sampler> sampler = CreateSharedRef<UniformSampler>(samples_per_pixel, 1234);
    PathTracer pt(sampler, max_bounces);

    timer.Mark();
    double t = timer.Get();
    std::cout << "Scene construction: " << t << "s" << std::endl;

    Film film(width, height);
    pt.Preprocess(scene, *camera);
    pt.Render(&film, scene, *camera);

    timer.Mark();
    t = timer.Get();

    std::cout << "\nDone!: " << t << 's' << std::endl;

    Bitmap bitmap = film.ConvertToBitmap();

    std::string filename = std::format("render_{}x{}_s{}_d{}_t{}s.png", width, height, samples_per_pixel, max_bounces, t);
    bitmap.WriteToFile(filename.c_str());

#if _DEBUG
    return 0;
#else
    return system(filename.c_str());
#endif
}