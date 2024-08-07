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

    ThreadPool::global_thread_pool.reset(new ThreadPool(std::thread::hardware_concurrency()));

    Scene scene;
    std::unique_ptr<Camera> camera;

    std::cout << "Loading scene.." << std::endl;
    Timer timer;
    if (!Sample::Get("head", &scene, &camera))
    {
        std::cout << "sample not found!" << std::endl;
        return 0;
    }

    timer.Mark();
    double t = timer.Get();
    std::cout << "Scene loading: " << t << "s" << std::endl;
    std::cout << "Primitives: " << scene.GetPrimitives().size() << std::endl;

    std::cout << "Building Acceleration structure.." << std::endl;
    BVH accel(scene.GetPrimitives());
    timer.Mark();
    t = timer.Get();
    std::cout << "Acceleration structure build: " << t << "s" << std::endl;

    int32 samples_per_pixel = 64;
    int32 max_bounces = 100;

    // IndependentSampler sampler(samples_per_pixel);
    StratifiedSampler sampler(std::sqrt(samples_per_pixel), std::sqrt(samples_per_pixel), true);

    VolPathIntegrator renderer(&accel, scene.GetLights(), &sampler, max_bounces);
    // PathIntegrator renderer(&accel, scene.GetLights(), &sampler, max_bounces);
    // DebugIntegrator renderer(&accel, scene.GetLights(), &sampler);
    // AmbientOcclusion renderer(&accel, scene.GetLights(), &sampler, 0.5f);
    // AlbedoIntegrator renderer(&accel, scene.GetLights(), &sampler);
    // WhittedStyle renderer(&accel, scene.GetLights(), &sampler, max_bounces);

    std::unique_ptr<RenderingProgress> rendering = renderer.Render(*camera);
    const Film& film = rendering->WaitAndLogProgress();

    timer.Mark();
    t = timer.Get();
    std::cout << "\nComplete: " << t << 's' << std::endl;

    Bitmap bitmap = film.ConvertToBitmap();

    auto [width, height] = camera->GetScreenResolution();
    std::string filename = std::format("render_{}x{}_s{}_d{}_t{}s.png", width, height, samples_per_pixel, max_bounces, t);
    bitmap.WriteToFile(filename.c_str());

#if _DEBUG
    return 0;
#else
    return system(filename.c_str());
#endif
}