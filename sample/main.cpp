#if defined(_WIN32) && defined(_DEBUG)
#include <crtdbg.h>
#endif

#include "bulbit/bulbit.h"
#include "samples.h"

#include <format>

int main(int argc, char* argv[])
{
#if defined(_WIN32) && defined(_DEBUG)
    // Enable memory-leak reports
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    using namespace bulbit;

    Scene scene;
    std::unique_ptr<Camera> camera;

    Timer timer;
    if (!Sample::Get("cornell-box", &scene, &camera))
    {
        std::cout << "sample not found!" << std::endl;
        return 0;
    }

    timer.Mark();
    double t = timer.Get();
    std::cout << "Scene construction: " << t << "s" << std::endl;

    scene.BuildAccelerationStructure();
    timer.Mark();
    t = timer.Get();
    std::cout << "Acceleration structure build: " << t << "s" << std::endl;

    int32 samples_per_pixel = 64;
    int32 max_bounces = 50;
    Ref<Sampler> sampler = std::make_shared<IndependentSampler>(samples_per_pixel);
    PathIntegrator renderer(sampler, max_bounces);
    // WhittedStyle renderer(sampler, max_bounces);
    // DebugIntegrator renderer(sampler);
    // AmbientOcclusion renderer(sampler, 0.5f);

    Film film(camera.get());
    renderer.Preprocess(scene, *camera);
    renderer.Render(&film, scene, *camera);

    timer.Mark();
    t = timer.Get();
    std::cout << "Rendering: " << t << 's' << std::endl;

    Bitmap bitmap = film.ConvertToBitmap();

    int32 width = camera->GetScreenWidth();
    int32 height = camera->GetScreenHeight();
    std::string filename = std::format("render_{}x{}_s{}_d{}_t{}s.png", width, height, samples_per_pixel, max_bounces, t);
    bitmap.WriteToFile(filename.c_str());

#if _DEBUG
    return 0;
#else
    return system(filename.c_str());
#endif
}