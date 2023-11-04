#if defined(_WIN32) && defined(_DEBUG)
#include <crtdbg.h>
#endif

#include "bulbit/bulbit.h"
#include "samples.h"

#include <format>

int main()
{
#if defined(_WIN32) && defined(_DEBUG)
    // Enable memory-leak reports
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    using namespace bulbit;

    Scene scene;
    Camera* camera;

    Timer timer;
    if (!Sample::Get("cornell-box", &scene, &camera))
    {
        std::cout << "sample not found!" << std::endl;
        return 0;
    }

    int32 samples_per_pixel = 64;
    int32 max_bounces = 50;
    Ref<Sampler> sampler = CreateSharedRef<UniformSampler>(samples_per_pixel);
    PathIntegrator renderer(sampler, max_bounces);

    timer.Mark();
    double t = timer.Get();
    std::cout << "Scene construction: " << t << "s" << std::endl;

    Film film(camera);
    renderer.Preprocess(scene, *camera);
    renderer.Render(&film, scene, *camera);

    timer.Mark();
    t = timer.Get();

    std::cout << "Done rendering!: " << t << 's' << std::endl;

    Bitmap bitmap = film.ConvertToBitmap();

    std::string filename = std::format("render_{}x{}_s{}_d{}_t{}s.png", camera->GetScreenWidth(), camera->GetScreenHeight(),
                                       samples_per_pixel, max_bounces, t);
    bitmap.WriteToFile(filename.c_str());

    delete camera;

#if _DEBUG
    return 0;
#else
    return system(filename.c_str());
#endif
}