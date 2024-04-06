#include "../samples.h"
#include "bulbit/perspective_camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

std::unique_ptr<Camera> ShipScene(Scene& scene)
{
    // Table
    {
        auto tf = Transform{ Point3(0.0f, -2.2f, 0.0f), Quat(DegToRad(0.0f), y_axis), Vec3(5.0f) };
        auto model = std::make_shared<Model>("res/wooden_table_02_4k/wooden_table_02_4k.gltf", tf);

        scene.AddModel(model);
    }

    // Ship
    {
        auto tf = Transform{ Point3(0.0f, 2.0f, 0.0f), Quat(DegToRad(90.0f), y_axis), Vec3(0.1f) };
        auto model = std::make_shared<Model>("res/ship_pinnace_4k/ship_pinnace_4k.gltf", tf);

        scene.AddModel(model);
    }

    // Light
    {
        Float size = 0.5;

        auto white = scene.CreateMaterial<DiffuseLight>(Spectrum(50.0f), true);
        auto tf = Transform{ Point3(0.0f, 6.0f, -3.0f), Quat(pi / 4.0f, x_axis), Vec3(size) };
        auto rect = CreateRectXY(tf, white);

        scene.AddMesh(rect);

        tf = Transform{ Point3(0.0f, 6.0f, 3.0f), Quat(pi - pi / 4.0f, x_axis), Vec3(size) };
        rect = CreateRectXY(tf, white);

        scene.AddMesh(rect);

        tf = Transform{ Point3(-3.0f, 6.0f, 0.0f), Quat(-pi / 4.0f, z_axis), Vec3(size) };
        rect = CreateRectYZ(tf, white);

        scene.AddMesh(rect);

        tf = Transform{ Point3(3.0f, 6.0f, 0.0f), Quat(pi + pi / 4.0f, z_axis), Vec3(size) };
        rect = CreateRectYZ(tf, white);

        scene.AddMesh(rect);
    }

    // Floor
    {
        auto mat = scene.CreateMaterial<Microfacet>(ConstantColor::Create(1.0), ConstantColor::Create(Spectrum(0.0f)),
                                                    ConstantColor::Create(Spectrum(0.001f)));
        Float size = 9.0f;
        Float y = 2.1f;

        auto tf = Transform{ Point3(0.0f, y - size / 2.0f, 0.0f), identity, Vec3(size) };
        auto rect = CreateRectXZ(tf, mat);
        scene.AddMesh(rect);

        tf = Transform{ Point3(-size / 2.0f, y, 0.0f), identity, Vec3(size) };
        rect = CreateRectYZ(tf, mat);
        scene.AddMesh(rect);

        tf = Transform{ Point3(0.0f, y, -size / 2.0f), identity, Vec3(size) };
        rect = CreateRectXY(tf, mat);
        scene.AddMesh(rect);

        tf = Transform{ Point3(0.0f, y - size / 2.0f, 0.0f), identity, Vec3(size) };
        rect = CreateRectXZ(tf, mat);
        scene.AddMesh(rect);
    }

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 1920;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 5, 5, 10 };
    Point3 lookat{ 0, 2.9f, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 25;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("ship", ShipScene);

} // namespace bulbit