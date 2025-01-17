#include "../samples.h"

std::unique_ptr<Camera> SSSTest2(Scene& scene)
{
    SetLoaderUseForceFallbackMaterial(true);

    // Floor
    {
        auto a = ConstantColorTexture::Create(0.75, 0.75, 0.75);
        auto b = ConstantColorTexture::Create(0.3, 0.3, 0.3);
        auto checker = ColorCheckerTexture::Create(a, b, Point2(20));
        auto tf = Transform{ Vec3(0, 0, 0), Quat::FromEuler({ 0, 0, 0 }), Vec3(3) };
        auto floor = scene.CreateMaterial<DiffuseMaterial>(checker);
        SetLoaderFallbackMaterial(floor);
        LoadModel(scene, "res/background.obj", tf);
    }

    // Model
    {
        Float d = 0.65;

        Spectrum r(0, 163 / 255.0, 108 / 255.0);
        Spectrum ssc(1);

        auto tf = Transform{ Vec3(-2 * d, 0, 0), Quat::FromEuler({ 0, 0, 0 }), Vec3(1) };
        auto mat = scene.CreateMaterial<SubsurfaceMaterialDiffusion>(r, ssc * 0.001, 1.5f, 0.0f);
        SetLoaderFallbackMaterial(mat);
        LoadModel(scene, "res/stanford/lucy.obj", tf);

        tf = Transform{ Vec3(-1 * d, 0, 0), Quat::FromEuler({ 0, 0, 0 }), Vec3(1) };
        mat = scene.CreateMaterial<SubsurfaceMaterialDiffusion>(r, ssc * 0.005, 1.5f, 0.0f);
        SetLoaderFallbackMaterial(mat);
        LoadModel(scene, "res/stanford/lucy.obj", tf);

        tf = Transform{ Vec3(0, 0, 0), Quat::FromEuler({ 0, 0, 0 }), Vec3(1) };
        mat = scene.CreateMaterial<SubsurfaceMaterialDiffusion>(r, ssc * 0.008, 1.5f, 0.0f);
        SetLoaderFallbackMaterial(mat);
        LoadModel(scene, "res/stanford/lucy.obj", tf);

        tf = Transform{ Vec3(1 * d, 0, 0), Quat::FromEuler({ 0, 0, 0 }), Vec3(1) };
        mat = scene.CreateMaterial<SubsurfaceMaterialDiffusion>(r, ssc * 0.01, 1.5f, 0.0f);
        SetLoaderFallbackMaterial(mat);
        LoadModel(scene, "res/stanford/lucy.obj", tf);

        tf = Transform{ Vec3(2 * d, 0, 0), Quat::FromEuler({ 0, 0, 0 }), Vec3(1) };
        mat = scene.CreateMaterial<SubsurfaceMaterialDiffusion>(r, ssc * 0.02, 1.5f, 0.0f);
        SetLoaderFallbackMaterial(mat);
        LoadModel(scene, "res/stanford/lucy.obj", tf);
    }

    {
        auto tf = Transform{ 0, 3.0f, 0, Quat(pi, x_axis), Vec3(5.0f, 1, 1) };
        auto light = scene.CreateMaterial<DiffuseLightMaterial>(Spectrum(5.0f));
        CreateRectXZ(scene, tf, light);
    }

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 9.f / 16.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 960;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0, 1.5, 3.5 };
    Point3 lookat{ 0.0, 0.5, 0.0 };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0.01f;
    Float vFov = 30.0;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus, Point2i(width, height));
}

static int32 index = Sample::Register("sss2", SSSTest2);
