#include "../samples.h"
#include "bulbit/camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"

std::unique_ptr<Camera> ShipScene(Scene& scene)
{
    // Table
    {
        auto tf = Transform{ Point3(0.0f, -2.2f, 0.0f), Quat(DegToRad(0.0f), y_axis), Vec3(5.0f) };
        LoadModel(scene, "res/wooden_table_02_4k/wooden_table_02_4k.gltf", tf);
    }

    // Ship
    {
        auto tf = Transform{ Point3(0.0f, 2.0f, 0.0f), Quat(DegToRad(90.0f), y_axis), Vec3(0.1f) };
        LoadModel(scene, "res/ship_pinnace_4k/ship_pinnace_4k.gltf", tf);
    }

    // Light
    {
        Float size = 0.5;

        auto white = scene.CreateMaterial<DiffuseLightMaterial>(Spectrum(50.0f), true);
        auto tf = Transform{ Point3(0.0f, 6.0f, -3.0f), Quat(pi / 4.0f, x_axis), Vec3(size) };
        CreateRectXY(scene, tf, white);

        tf = Transform{ Point3(0.0f, 6.0f, 3.0f), Quat(pi - pi / 4.0f, x_axis), Vec3(size) };
        CreateRectXY(scene, tf, white);

        tf = Transform{ Point3(-3.0f, 6.0f, 0.0f), Quat(-pi / 4.0f, z_axis), Vec3(size) };
        CreateRectYZ(scene, tf, white);

        tf = Transform{ Point3(3.0f, 6.0f, 0.0f), Quat(pi + pi / 4.0f, z_axis), Vec3(size) };
        CreateRectYZ(scene, tf, white);
    }

    // Floor
    {
        auto mat = scene.CreateMaterial<UnrealMaterial>(
            ConstantColorTexture::Create(1.0), ConstantFloatTexture::Create(0.0f), ConstantFloatTexture::Create(0.1f)
        );
        Float size = 9.0f;
        Float y = 2.1f;

        auto tf = Transform{ Point3(0.0f, y - size / 2.0f, 0.0f), identity, Vec3(size) };
        CreateRectXZ(scene, tf, mat);

        tf = Transform{ Point3(-size / 2.0f, y, 0.0f), identity, Vec3(size) };
        CreateRectYZ(scene, tf, mat);

        tf = Transform{ Point3(0.0f, y, -size / 2.0f), identity, Vec3(size) };
        CreateRectXY(scene, tf, mat);

        tf = Transform{ Point3(0.0f, y - size / 2.0f, 0.0f), identity, Vec3(size) };
        CreateRectXZ(scene, tf, mat);
    }

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 960;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 5, 5, 10 };
    Point3 lookat{ 0, 2.9f, 0 };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0;
    Float vFov = 25;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("ship", ShipScene);
