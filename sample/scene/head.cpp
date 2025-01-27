#include "../samples.h"

std::unique_ptr<Camera> Head(Scene& scene)
{
    SetLoaderUseForceFallbackMaterial(true);
    // Head
    {
        auto head_albedo = CreateSpectrumImageTexture(scene, "res/head/lambertian.jpg");
        auto mat =
            CreateSubsurfaceRandomWalkMaterial(scene, head_albedo, Spectrum(0.0012953, 0.00095238, 0.00067114), 1.33f, 0.1f);
        // auto mat = scene.CreateMaterial<DiffuseMaterial>(head_albedo);
        SetLoaderFallbackMaterial(mat);

        auto tf = Transform{ Vec3(0.0f, 0.05f, 0.0f), Quat(DegToRad(0.0f), y_axis), Vec3(1.5f) };
        LoadModel(scene, "res/head/head.obj", tf);
    }

    // CreateImageInfiniteLight(scene, "res/HDR/small_rural_road_1k.hdr", Transform(Quat(0, y_axis)), 2);
    CreateImageInfiniteLight(scene, "res/HDR/material-test.hdr", Transform(Quat(pi / 2, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/sunset.hdr", Transform(Quat(-pi / 2, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/aerodynamics_workshop_1k.hdr", Transform(Quat(pi, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/scythian_tombs_2_4k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/quarry_04_puresky_1k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/photo_studio_01_1k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/peppermint_powerplant_4k.hdr", Transform(Quat(0, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/white_cliff_top_1k.hdr", Transform(Quat(pi, y_axis)));
    // CreateImageInfiniteLight(scene, "res/sunflowers/sunflowers_puresky_4k.hdr");
    // CreateImageInfiniteLight(scene, "res/HDR/san_giuseppe_bridge_4k.hdr", Transform(Quat(pi / 2, y_axis)));
    // CreateImageInfiniteLight(scene, "res/HDR/Background_05.hdr", Transform(Quat(pi / 2, y_axis)));
    // CreateUniformInfiniteLight(scene, Spectrum(1));

    // Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 9.f / 16.f;
    // Float aspect_ratio = 3.f / 2.f;
    Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 1000;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom = Point3{ -0.3, 0.1, 1 } * 0.8;
    Point3 lookat = Point3{ 0, 0, 0 };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0.0f;
    Float vFov = 30;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus, Point2i(width, height));
}

static int32 index = Sample::Register("head", Head);
