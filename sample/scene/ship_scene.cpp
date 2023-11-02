#include "../samples.h"
#include "bulbit/diffuse_light.h"
#include "bulbit/lambertian.h"
#include "bulbit/perspective_camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

Camera* ShipScene(Scene& scene)
{
    // Table
    {
        auto tf = Transform{ Point3(0.0f, -2.2f, 0.0f), Quat(DegToRad(0.0f), y_axis), Vec3(5.0f) };
        auto model = CreateSharedRef<Model>("res/wooden_table_02_4k/wooden_table_02_4k.gltf", tf);

        scene.Add(model);
    }

    // Ship
    {
        auto tf = Transform{ Point3(0.0f, 2.0f, 0.0f), Quat(DegToRad(90.0f), y_axis), Vec3(0.1f) };
        auto model = CreateSharedRef<Model>("res/ship_pinnace_4k/ship_pinnace_4k.gltf", tf);

        scene.Add(model);
    }

    // Light
    {
        Float size = 0.5;

        auto white = CreateSharedRef<DiffuseLight>(ConstantColor::Create(Spectrum(30.0f)));
        white->two_sided = true;
        auto tf = Transform{ Point3(0.0f, 5.0f, -3.0f), Quat(pi / 4.0f, x_axis), Vec3(size) };
        auto rect = CreateRectXY(tf, white);

        scene.AddLight(rect);

        tf = Transform{ Point3(0.0f, 5.0f, 3.0f), Quat(pi - pi / 4.0f, x_axis), Vec3(size) };
        rect = CreateRectXY(tf, white);

        scene.AddLight(rect);

        tf = Transform{ Point3(-3.0f, 5.0f, 0.0f), Quat(-pi / 4.0f, z_axis), Vec3(size) };
        rect = CreateRectYZ(tf, white);

        scene.AddLight(rect);

        tf = Transform{ Point3(3.0f, 5.0f, 0.0f), Quat(pi + pi / 4.0f, z_axis), Vec3(size) };
        rect = CreateRectYZ(tf, white);

        scene.AddLight(rect);
    }

    // Floor
    {
        auto mat = CreateSharedRef<Microfacet>(ConstantColor::Create(1.0), ConstantColor::Create(Spectrum(0.0f)),
                                               ConstantColor::Create(Spectrum(0.001f)));
        Float size = 9.0f;
        Float y = 2.1f;

        auto tf = Transform{ Point3(0.0f, y - size / 2.0f, 0.0f), identity, Vec3(size) };
        auto rect = CreateRectXZ(tf, mat);
        scene.Add(rect);

        tf = Transform{ Point3(-size / 2.0f, y, 0.0f), identity, Vec3(size) };
        rect = CreateRectYZ(tf, mat);
        scene.Add(rect);

        tf = Transform{ Point3(0.0f, y, -size / 2.0f), identity, Vec3(size) };
        rect = CreateRectXY(tf, mat);
        scene.Add(rect);

        tf = Transform{ Point3(0.0f, y - size / 2.0f, 0.0f), identity, Vec3(size) };
        rect = CreateRectXZ(tf, mat);
        scene.Add(rect);
    }

    Float aspect_ratio = 16.0f / 9.0f;

    Point3 lookfrom{ 5, 5, 10 };
    Point3 lookat{ 0, 2.8f, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 30;

    return new PerspectiveCamera(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
}

static int32 index = Sample::Register("ship", ShipScene);

} // namespace bulbit