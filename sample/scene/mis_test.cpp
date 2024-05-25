#include "../samples.h"
#include "bulbit/camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"

std::unique_ptr<Camera> MISTest(Scene& scene)
{
    {
        auto floor_mat = scene.CreateMaterial<DiffuseMaterial>(Spectrum(0.4f));
        SetLoaderFallbackMaterial(floor_mat);
        LoadModel(scene, "res/veach_mi/floor.obj", Transform{ identity });
    }

    // plates
    {
        auto m1 = scene.CreateMaterial<UnrealMaterial>(ConstantColorTexture::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                                       ConstantFloatTexture::Create(1.0f),
                                                       ConstantFloatTexture::Create(std::sqrt(0.005f)));

        auto m2 = scene.CreateMaterial<UnrealMaterial>(ConstantColorTexture::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                                       ConstantFloatTexture::Create(1.0f),
                                                       ConstantFloatTexture::Create(std::sqrt(0.02f)));

        auto m3 = scene.CreateMaterial<UnrealMaterial>(ConstantColorTexture::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                                       ConstantFloatTexture::Create(1.0f),
                                                       ConstantFloatTexture::Create(std::sqrt(0.05f)));

        auto m4 = scene.CreateMaterial<UnrealMaterial>(ConstantColorTexture::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                                       ConstantFloatTexture::Create(1.0f),
                                                       ConstantFloatTexture::Create(std::sqrt(0.1f)));

        SetLoaderFallbackMaterial(m1);
        LoadModel(scene, "res/veach_mi/plate1.obj", Transform{ identity });
        SetLoaderFallbackMaterial(m2);
        LoadModel(scene, "res/veach_mi/plate2.obj", Transform{ identity });
        SetLoaderFallbackMaterial(m3);
        LoadModel(scene, "res/veach_mi/plate3.obj", Transform{ identity });
        SetLoaderFallbackMaterial(m4);
        LoadModel(scene, "res/veach_mi/plate4.obj", Transform{ identity });
    }

    // Lights
    {
        auto light1 = scene.CreateMaterial<DiffuseLight>(Spectrum(800.f));
        auto light3 = scene.CreateMaterial<DiffuseLight>(Spectrum(901.803f));
        auto light2 = scene.CreateMaterial<DiffuseLight>(Spectrum(100.f));
        auto light4 = scene.CreateMaterial<DiffuseLight>(Spectrum(11.1111f));
        auto light5 = scene.CreateMaterial<DiffuseLight>(Spectrum(1.23457f));

        auto s1 = scene.CreatePrimitive<Sphere>(Vec3(10, 10, 4), 0.5f, light1);
        auto s3 = scene.CreatePrimitive<Sphere>(Vec3(-3.75f, 0, 0), 0.03333f, light3);
        auto s2 = scene.CreatePrimitive<Sphere>(Vec3(-1.25f, 0, 0), 0.1f, light2);
        auto s4 = scene.CreatePrimitive<Sphere>(Vec3(1.25f, 0, 0), 0.3f, light4);
        auto s5 = scene.CreatePrimitive<Sphere>(Vec3(3.75f, 0, 0), 0.9f, light5);
        scene.CreateLight<AreaLight>(s1);
        scene.CreateLight<AreaLight>(s3);
        scene.CreateLight<AreaLight>(s2);
        scene.CreateLight<AreaLight>(s4);
        scene.CreateLight<AreaLight>(s5);
    }

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0, 2, 15 };
    Point3 lookat{ 0, -2, 2.5f };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 28;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index1 = Sample::Register("mis", MISTest);

std::unique_ptr<Camera> MISTestWak(Scene& scene)
{
    {
        auto floor = scene.CreateMaterial<DiffuseMaterial>(ColorImageTexture::Create("res/wakdu.jpg"));
        Float s = 20.0f;
        auto tf = Transform{ Vec3(0.0f, -4.0f, -4.0f), identity, Vec3(s, 1.0f, s) };
        CreateRectXZ(scene, tf, floor);

        tf = Transform{ Vec3(0.0f, -4.0f, -4.0f), identity, Vec3(s, s, 1.0f) };
        CreateRectXY(scene, tf, floor);
    }

    // plates
    {
        auto m1 = scene.CreateMaterial<UnrealMaterial>(ConstantColorTexture::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                                       ConstantFloatTexture::Create(1.0f),
                                                       ConstantFloatTexture::Create(std::sqrt(0.005f)));

        auto m2 = scene.CreateMaterial<UnrealMaterial>(ConstantColorTexture::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                                       ConstantFloatTexture::Create(1.0f),
                                                       ConstantFloatTexture::Create(std::sqrt(0.02f)));

        auto m3 = scene.CreateMaterial<UnrealMaterial>(ConstantColorTexture::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                                       ConstantFloatTexture::Create(1.0f),
                                                       ConstantFloatTexture::Create(std::sqrt(0.05f)));

        auto m4 = scene.CreateMaterial<UnrealMaterial>(ConstantColorTexture::Create(Spectrum(0.07f, 0.09f, 0.13f)),
                                                       ConstantFloatTexture::Create(1.0f),
                                                       ConstantFloatTexture::Create(std::sqrt(0.1f)));

        SetLoaderFallbackMaterial(m1);
        LoadModel(scene, "res/veach_mi/plate1.obj", Transform{ identity });
        SetLoaderFallbackMaterial(m2);
        LoadModel(scene, "res/veach_mi/plate2.obj", Transform{ identity });
        SetLoaderFallbackMaterial(m3);
        LoadModel(scene, "res/veach_mi/plate3.obj", Transform{ identity });
        SetLoaderFallbackMaterial(m4);
        LoadModel(scene, "res/veach_mi/plate4.obj", Transform{ identity });
    }

    // Lights
    {
        auto light1 = scene.CreateMaterial<DiffuseLight>(Spectrum(800));
        auto light3 = scene.CreateMaterial<DiffuseLight>(Spectrum(901.803f, 0, 0));
        auto light2 = scene.CreateMaterial<DiffuseLight>(Spectrum(100, 100 / 5, 0));
        auto light4 = scene.CreateMaterial<DiffuseLight>(Spectrum(11.1111f, 11.1111f, 0));
        auto light5 = scene.CreateMaterial<DiffuseLight>(Spectrum(0, 1.23457f, 0));

        auto s1 = scene.CreatePrimitive<Sphere>(Vec3(10, 10, 4), 0.5f, light1);
        auto s3 = scene.CreatePrimitive<Sphere>(Vec3(-3.75f, 0, 0), 0.03333f, light3);
        auto s2 = scene.CreatePrimitive<Sphere>(Vec3(-1.25f, 0, 0), 0.1f, light2);
        auto s4 = scene.CreatePrimitive<Sphere>(Vec3(1.25f, 0, 0), 0.3f, light4);
        auto s5 = scene.CreatePrimitive<Sphere>(Vec3(3.75f, 0, 0), 0.9f, light5);
        scene.CreateLight<AreaLight>(s1);
        scene.CreateLight<AreaLight>(s3);
        scene.CreateLight<AreaLight>(s2);
        scene.CreateLight<AreaLight>(s4);
        scene.CreateLight<AreaLight>(s5);
    }

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0, 2, 15 };
    Point3 lookat{ 0, -2, 2.5f };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 28;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index2 = Sample::Register("mis-wak", MISTestWak);
