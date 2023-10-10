#include "spt/spt.h"

namespace spt
{

void StatueScene(Scene& scene)
{
    {
        auto mat = RandomMicrofacetMaterial();
        mat->basecolor = ConstantColor::Create(Spectrum(1.0));
        mat->metallic = ConstantColor::Create(Spectrum(1.0));
        mat->roughness = ConstantColor::Create(Spectrum(0.1));

        // auto mat = CreateSharedRef<Dielectric>(1.5);

        Material::fallback = mat;

        auto tf = Transform{ Point3(0.0, -2.0, 0.0), Quat(DegToRad(45.0), y_axis), Vec3(20.0) };
        auto model = CreateSharedRef<Model>("res/horse_statue_01_4k/horse_statue_01_4k.gltf", tf);

        // auto tf = Transform{ Point3(0.0, -2.0, 0.0), Quat(DegToRad(0.0), y_axis), Vec3(8.0) };
        // auto model = CreateSharedRef<Model>("res/marble_bust_01_4k/marble_bust_01_4k.gltf", tf);

        scene.Add(model);
    }

    Float light = 8.0;
    Float distance = 5.0;
    Float size = 2.0;

    {
        auto red = CreateSharedRef<DiffuseLight>(ConstantColor::Create(Spectrum(light, 0.0, 0.0)));
        auto tf = Transform{ Point3(-distance, 0.0, 0.0), Quat(identity), Vec3(1.0, size, size) };
        auto rect = CreateRectYZ(tf, red);

        scene.AddLight(rect);
    }

    {
        auto blue = CreateSharedRef<DiffuseLight>(ConstantColor::Create(Spectrum(0.0, 0.0, light)));
        auto tf = Transform{ Point3(distance, 0.0, 0.0), Quat(pi, y_axis), Vec3(1.0, size, size) };
        auto rect = CreateRectYZ(tf, blue);

        scene.AddLight(rect);
    }

    {
        auto white = CreateSharedRef<DiffuseLight>(ConstantColor::Create(Spectrum(0.5)));

        auto tf = Transform{ Point3(0.0, 4.0, 0.0), Quat(pi, x_axis), Vec3(8.0, 1.0, 8.0) };
        auto rect = CreateRectXZ(tf, white);

        scene.AddLight(rect);
    }

    // {
    // auto white = CreateSharedRef<DiffuseLight>(SolidColor::Create(Spectrum(3.0)));

    //     int32 count = 10;
    //     Float d = two_pi / count;

    //     Float y = 2.0;
    //     Float r = 1.5;

    //     for (int i = 0; i < count; ++i)
    //     {
    //         Float angle = d * i;
    //         auto pos = Vec3(std::cos(angle) * r, y, std::sin(angle) * r);
    //         auto sphere = CreateSharedRef<Sphere>(pos, 0.1, white);

    //         scene.AddLight(sphere);
    //     }
    // }

    // Floor
    {
        auto mat = RandomMicrofacetMaterial();
        mat->basecolor = ConstantColor::Create(Spectrum(1.0));
        mat->metallic = ConstantColor::Create(Spectrum(0.0));
        mat->roughness = ConstantColor::Create(Spectrum(0.01));

        auto tf = Transform{ Point3(0.0, -2.0, 0.0), Quat(identity), Vec3(8.0, 1.0, 8.0) };
        auto rect = CreateRectXZ(tf, mat);

        scene.Add(rect);

        // mat = RandomPBRMaterial();
        // mat->basecolor_map = SolidColor::Create(Vec3(1.0));
        // mat->metallic_map = SolidColor::Create(Vec3(1.0));
        // mat->roughness_map = SolidColor::Create(Vec3(0.05));

        tf = Transform{ Point3(0.0, 0.0, -4.0), Quat(identity), Vec3(8.0, 8.0, 1.0) };
        rect = CreateRectXY(tf, mat);

        scene.Add(rect);
    }

    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/solitude_night_4k/solitude_night_4k.hdr");
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr");
}

} // namespace spt