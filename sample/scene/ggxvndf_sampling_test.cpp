#include "../samples.h"

std::unique_ptr<Camera> GGXVNDFSamplingTest(Scene& scene)
{
    // Bunny
    {
        Srand(123123);
        auto mat = CreateRandomUnrealMaterial(scene);
        // auto mat = scene.CreateMaterial<MirrorMaterial>(Spectrum(0.7f));
        // auto mat = scene.CreateMaterial<DielectricMaterial>(1.5f, FloatConstantTexture::Create(0.0f));
        // auto mat = scene.CreateMaterial<ThinDielectricMaterial>(1.5f);
        // auto mat = scene.CreateMaterial<ConductorMaterial>(
        //     ColorConstantTexture::Create(0.1, 0.2, 1.9), ColorConstantTexture::Create(3, 2.5, 2),
        //     FloatConstantTexture::Create(0.1f), FloatConstantTexture::Create(0.3f));
        // auto mix = scene.CreateMaterial<MixtureMaterial>(mat, mat, 0.1f);

        auto tf = Transform{ Vec3::zero, Quat(DegToRad(0.0f), y_axis), Vec3(3.0f) };
        SetLoaderFallbackMaterial(mat);
        LoadModel(scene, "res/stanford/bunny.obj", tf);
    }

    // scene.CreateLight<ImageInfiniteLight>("res/HDR/scythian_tombs_2_4k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/HDR/quarry_04_puresky_1k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/solitude_night_4k/solitude_night_4k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/sunflowers/sunflowers_puresky_4k.hdr");
    scene.CreateLight<ImageInfiniteLight>("res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(-pi, y_axis)));

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 960;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0, 2, 10 };
    Point3 lookat{ 0, 1, 0 };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0;
    Float vFov = 30;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus, Point2i(width, height));
}

static int32 index = Sample::Register("ggxvndf", GGXVNDFSamplingTest);
