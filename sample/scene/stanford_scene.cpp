#include "../samples.h"
#include "bulbit/camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

std::unique_ptr<Camera> StanfordScene(Scene& scene)
{
    // Floor
    {
        auto mat = scene.CreateMaterial<Microfacet>(
            ImageTexture::Create("res/dark_wooden_planks_4k/textures/dark_wooden_planks_diff_4k.jpg"),
            ImageTexture::Create("res/dark_wooden_planks_4k/textures/dark_wooden_planks_arm_4k.jpg"),
            ImageTexture::Create("res/dark_wooden_planks_4k/textures/dark_wooden_planks_arm_4k.jpg"), ConstantColor::Create(0.0f),
            ImageTexture::Create("res/dark_wooden_planks_4k/textures/dark_wooden_planks_nor_gl_4k.png"));

        auto tf = Transform{ Vec3::zero, identity, Vec3(8.0f) };
        CreateRectXZ(scene, tf, mat, Point2(4.0f, 4.0f));
    }

    Float scale = 0.3f;
    Float gap = 0.2f;

    // Bunny
    {
        auto tf = Transform{ Vec3(gap * 3.0f, 0.0f, 0.0f), Quat(0.0f, y_axis), Vec3(scale) };
        auto mat = CreateRandomMicrofacetMaterial(scene);

        LoadModel(scene, "res/stanford/bunny.obj", tf, mat);
    }

    // Lucy
    {
        auto tf = Transform{ Vec3(gap, 0.0f, 0.0f), Quat(0.0f, y_axis), Vec3(scale) };
        auto mat = CreateRandomMicrofacetMaterial(scene);

        LoadModel(scene, "res/stanford/lucy.obj", tf, mat);
    }

    // Tyrannosaurus
    {
        auto tf = Transform{ Vec3(-gap, 0.0f, 0.0f), Quat(DegToRad(45.0f), y_axis), Vec3(scale) };
        auto mat = CreateRandomMicrofacetMaterial(scene);

        LoadModel(scene, "res/stanford/tyra.obj", tf, mat);
    }

    // Armadillo
    {
        auto tf = Transform{ Vec3(-gap * 3.0f, 0.0f, 0.0f), Quat(0.0f, y_axis), Vec3(scale) };
        auto mat = scene.CreateMaterial<Microfacet>(
            ConstantColor::Create(Spectrum(Rand(0.0f, 1.0f), Rand(0.0f, 1.0f), Rand(0.0f, 1.0f)) * Float(0.7f)),
            ConstantColor::Create(Spectrum(1.0f)), ConstantColor::Create(Spectrum(0.2f)));

        LoadModel(scene, "res/stanford/arma.obj", tf, mat);
    }

    {
        Float w = 0.04f;
        Float h = 0.6f;
        Float cx = 16.0f;
        Float xgap = 0.015f;
        Float xstep = 2.0f * w + xgap;

        auto light = scene.CreateMaterial<DiffuseLight>(Spectrum(3.0f), true);

        for (int32 x = 0; x < cx; ++x)
        {
            Vec3 pos;

            pos.y = 0.6f;
            pos.x = x * xstep - ((cx - 1) * xstep / 2.0f);
            pos.z = 0.0f;

            auto mat = CreateRandomMicrofacetMaterial(scene);

            auto tf = Transform{ pos, Quat(pi, x_axis), Vec3(w, w, h) };
            CreateRectXZ(scene, tf, light);
        }
    }

    // scene.CreateLight<ImageInfiniteLight>("res/sunflowers/sunflowers_puresky_4k.hdr"));

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 960;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0, 0.5f, 2 };
    Point3 lookat{ 0, 0.2f, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 30;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("stanford", StanfordScene);
