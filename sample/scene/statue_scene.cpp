#include "spt/spt.h"

namespace spt
{

void StatueScene(Scene& scene)
{
    {
        auto mat = CreateSharedRef<Microfacet>(ConstantColor::Create(1.0), ConstantColor::Create(Spectrum(1.0f)),
                                               ConstantColor::Create(Spectrum(0.1f)));
        // auto mat = CreateSharedRef<Dielectric>(1.5f);

        Material::fallback = mat;

        auto tf = Transform{ Point3(0.0f, -2.0f, 0.0f), Quat(DegToRad(45.0f), y_axis), Vec3(20.0f) };
        auto model = CreateSharedRef<Model>("res/horse_statue_01_4k/horse_statue_01_4k.gltf", tf);

        // auto tf = Transform{ Point3(0.0f, -2.0f, 0.0f), Quat(DegToRad(0.0f), y_axis), Vec3(8.0f) };
        // auto model = CreateSharedRef<Model>("res/marble_bust_01_4k/marble_bust_01_4k.gltf", tf);

        scene.Add(model);
    }

    Float light = 8.0f;
    Float distance = 5.0f;
    Float size = 2.0f;

    {
        auto red = CreateSharedRef<DiffuseLight>(ConstantColor::Create(Spectrum(light, 0.0f, 0.0f)));
        auto tf = Transform{ Point3(-distance, 0.0f, 0.0f), identity, Vec3(1.0f, size, size) };
        auto rect = CreateRectYZ(tf, red);

        scene.AddLight(rect);
    }

    {
        auto blue = CreateSharedRef<DiffuseLight>(ConstantColor::Create(Spectrum(0.0f, 0.0f, light)));
        auto tf = Transform{ Point3(distance, 0.0f, 0.0f), Quat(pi, y_axis), Vec3(1.0f, size, size) };
        auto rect = CreateRectYZ(tf, blue);

        scene.AddLight(rect);
    }

    {
        auto white = CreateSharedRef<DiffuseLight>(ConstantColor::Create(Spectrum(0.5f)));

        auto tf = Transform{ Point3(0.0f, 4.0f, 0.0f), Quat(pi, x_axis), Vec3(8.0f, 1.0f, 8.0f) };
        auto rect = CreateRectXZ(tf, white);

        scene.AddLight(rect);
    }

    // {
    // auto white = CreateSharedRef<DiffuseLight>(SolidColor::Create(Spectrum(3.0f)));

    //     int32 count = 10;
    //     Float d = two_pi / count;

    //     Float y = 2.0f;
    //     Float r = 1.5f;

    //     for (int i = 0; i < count; ++i)
    //     {
    //         Float angle = d * i;
    //         auto pos = Vec3(std::cos(angle) * r, y, std::sin(angle) * r);
    //         auto sphere = CreateSharedRef<Sphere>(pos, 0.1f, white);

    //         scene.AddLight(sphere);
    //     }
    // }

    // Floor
    {
        auto mat = CreateSharedRef<Microfacet>(ConstantColor::Create(1.0f), ConstantColor::Create(Spectrum(0.0f)),
                                               ConstantColor::Create(Spectrum(0.01f)));
        auto tf = Transform{ Point3(0.0f, -2.0f, 0.0f), identity, Vec3(8.0f, 1.0f, 8.0f) };
        auto rect = CreateRectXZ(tf, mat);

        scene.Add(rect);

        // mat = RandomPBRMaterial();
        // mat->basecolor_map = SolidColor::Create(Vec3(1.0f));
        // mat->metallic_map = SolidColor::Create(Vec3(1.0f));
        // mat->roughness_map = SolidColor::Create(Vec3(0.05));

        tf = Transform{ Point3(0.0f, 0.0f, -4.0f), identity, Vec3(8.0f, 8.0f, 1.0f) };
        rect = CreateRectXY(tf, mat);

        scene.Add(rect);
    }

    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/solitude_night_4k/solitude_night_4k.hdr");
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr");
}

} // namespace spt