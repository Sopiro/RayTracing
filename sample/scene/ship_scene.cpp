#include "spt/spt.h"

namespace spt
{

void ShipScene(Scene& scene)
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
        auto mat = RandomMicrofacetMaterial();
        mat->basecolor = ConstantColor::Create(Spectrum(1.0f));
        mat->metallic = ConstantColor::Create(Spectrum(0.0f));
        mat->roughness = ConstantColor::Create(Spectrum(0.001f));

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
}

} // namespace spt