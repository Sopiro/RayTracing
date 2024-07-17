#include "../samples.h"
#include "bulbit/camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"

std::unique_ptr<Camera> VolumeTest(Scene& scene)
{
    // Bunny
    {
        Srand(123123);
        // auto mat = CreateRandomUnrealMaterial(scene);
        // auto mat = scene.CreateMaterial<MirrorMaterial>(Spectrum(0.7f));
        // auto mat = scene.CreateMaterial<DielectricMaterial>(1.5f, ConstantFloatTexture::Create(0.02f));
        auto mat = scene.CreateMaterial<ThinDielectricMaterial>(1.5f);
        // auto mat = scene.CreateMaterial<ConductorMaterial>(
        //     ConstantColorTexture::Create(0.1, 0.2, 1.9), ConstantColorTexture::Create(3, 2.5, 2),
        //     ConstantFloatTexture::Create(0.1f), ConstantFloatTexture::Create(0.3f)
        // );
        // auto mix = scene.CreateMaterial<MixtureMaterial>(mat, mat, 0.1f);

        auto tf = Transform{ Vec3(0.0f, -1.0f, 0.0f), Quat(DegToRad(0.0f), y_axis), Vec3(4.0f) };
        SetLoaderUseForceFallbackMaterial(true);
        SetLoaderFallbackMaterial(nullptr);

        HomogeneousMedium* hm =
            scene.CreateMedium<HomogeneousMedium>(Spectrum(0.1, 0.2, 0.3), Spectrum(1, 5, 10), Spectrum(0.0), -0.9f);
        MediumInterface mi(hm, nullptr);
        SetLoaderFallbackMediumInterface(mi);
        LoadModel(scene, "res/stanford/lucy.obj", tf);
    }

    scene.CreateLight<ImageInfiniteLight>("res/HDR/scythian_tombs_2_4k.hdr", Transform(Quat(0, y_axis)));
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/quarry_04_puresky_1k.hdr", Transform(Quat(0, y_axis)));
    // scene.CreateLight<ImageInfiniteLight>("res/solitude_night_4k/solitude_night_4k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/sunflowers/sunflowers_puresky_4k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(-pi, y_axis)));
    // scene.CreateLight<UniformInfiniteLight>(Spectrum(1));

    // Float aspect_ratio = 16.f / 9.f;
    Float aspect_ratio = 9.f / 16.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 640;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0, 2, 10 };
    Point3 lookat{ 0, 1, 0 };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0;
    Float vFov = 30;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("volume", VolumeTest);
