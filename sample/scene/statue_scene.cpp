#include "spt/pathtracer.h"

namespace spt
{

void StatueScene(Scene& scene)
{
    {
        auto mat = RandomPBRMaterial();
        mat->basecolor_map = SolidColor::Create(Vec3{ 1.0 });
        mat->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        mat->roughness_map = SolidColor::Create(Vec3{ 0.1 });

        // auto mat = SharedRef<Dielectric>(1.5);

        Material::fallback_material = mat;

        auto tf = Transform{ Point3{ 0.0, -2.0, 0.0 }, Quat{ DegToRad(45.0), y_axis }, Vec3{ 20.0 } };
        auto model = CreateSharedRef<Model>("res/horse_statue_01_4k/horse_statue_01_4k.gltf", tf);

        // auto tf = Transform{ Point3{ 0.0, -2.0, 0.0 }, Quat{ DegToRad(0.0), y_axis }, Vec3{ 8.0 } };
        // auto model = SharedRef<Model>("res/marble_bust_01_4k/marble_bust_01_4k.gltf", tf);

        scene.Add(model);
    }

    double light = 8.0;
    double distance = 5.0;
    double size = 2.0;

    {
        auto red = CreateSharedRef<DiffuseLight>(SolidColor::Create(Color{ light, 0.0, 0.0 }));
        auto tf = Transform{ Point3{ -distance, 0.0, 0.0 }, Quat{ identity }, Vec3{ 1.0, size, size } };
        auto rect = RectYZ(tf, red);

        scene.Add(rect);
        scene.AddLight(rect);
    }

    {
        auto blue = CreateSharedRef<DiffuseLight>(SolidColor::Create(Color{ 0.0, 0.0, light }));
        auto tf = Transform{ Point3{ distance, 0.0, 0.0 }, Quat{ pi, y_axis }, Vec3{ 1.0, size, size } };
        auto rect = RectYZ(tf, blue);

        scene.Add(rect);
        scene.AddLight(rect);
    }

    {
        auto white = CreateSharedRef<DiffuseLight>(SolidColor::Create(Color{ 0.5 }));

        auto tf = Transform{ Point3{ 0.0, 4.0, 0.0 }, Quat{ pi, x_axis }, Vec3{ 8.0, 1.0, 8.0 } };
        auto rect = RectXZ(tf, white);

        scene.Add(rect);
        scene.AddLight(rect);
    }

    // {
    //     auto white = SharedRef<DiffuseLight>(SolidColor::Create(Color{ 3.0 }));

    //     int32 count = 10;
    //     double d = two_pi / count;

    //     double y = 2.0;
    //     double r = 1.5;

    //     for (int i = 0; i < count; ++i)
    //     {
    //         double angle = d * i;
    //         auto pos = Vec3{ cos(angle) * r, y, sin(angle) * r };
    //         auto sphere = SharedRef<Sphere>(pos, 0.1, white);

    //         scene.Add(sphere);
    //         scene.AddLight(sphere);
    //     }
    // }

    // Floor
    {
        auto mat = RandomPBRMaterial();
        mat->basecolor_map = SolidColor::Create(Vec3{ 1.0 });
        mat->metallic_map = SolidColor::Create(Vec3{ 0.0 });
        mat->roughness_map = SolidColor::Create(Vec3{ 0.01 });

        auto tf = Transform{ Point3{ 0.0, -2.0, 0.0 }, Quat{ identity }, Vec3{ 8.0, 1.0, 8.0 } };
        auto rect = RectXZ(tf, mat);

        scene.Add(rect);

        // mat = RandomPBRMaterial();
        // mat->basecolor_map = SolidColor::Create(Vec3{ 1.0 });
        // mat->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        // mat->roughness_map = SolidColor::Create(Vec3{ 0.05 });

        tf = Transform{ Point3{ 0.0, 0.0, -4.0 }, Quat{ identity }, Vec3{ 8.0, 8.0, 1.0 } };
        rect = RectXY(tf, mat);

        scene.Add(rect);
    }

    // scene.SetEnvironmentMap(ImageTexture::Create("res/solitude_night_4k/solitude_night_4k.hdr", false, true));
    scene.SetEnvironmentMap(ImageTexture::Create("res/sunflowers/sunflowers_puresky_4k.hdr", false, true));
    scene.SetEnvironmentMap(SolidColor::Create(Color{ 0.0 }));
}

} // namespace spt