#include "../samples.h"
#include "bulbit/diffuse_light.h"
#include "bulbit/lambertian.h"
#include "bulbit/perspective_camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

std::unique_ptr<Camera> StatueScene(Scene& scene)
{
    {
        auto mat = std::make_shared<Microfacet>(ConstantColor::Create(1.0), ConstantColor::Create(Spectrum(1.0f)),
                                                ConstantColor::Create(Spectrum(0.1f)));
        // auto mat = std::make_shared<Dielectric>(1.5f);

        Material::fallback = mat;

        auto tf = Transform{ Point3(0.0f, -2.0f, 0.0f), Quat(DegToRad(45.0f), y_axis), Vec3(20.0f) };
        auto model = std::make_shared<Model>("res/horse_statue_01_4k/horse_statue_01_4k.gltf", tf);

        // auto tf = Transform{ Point3(0.0f, -2.0f, 0.0f), Quat(DegToRad(0.0f), y_axis), Vec3(8.0f) };
        // auto model = std::make_shared<Model>("res/marble_bust_01_4k/marble_bust_01_4k.gltf", tf);

        scene.Add(model);
    }

    Float light = 8.0f;
    Float distance = 5.0f;
    Float size = 2.0f;

    {
        auto red = std::make_shared<DiffuseLight>(ConstantColor::Create(Spectrum(light, 0.0f, 0.0f)));
        auto tf = Transform{ Point3(-distance, 0.0f, 0.0f), identity, Vec3(1.0f, size, size) };
        auto rect = CreateRectYZ(tf, red);

        scene.AddLight(rect);
    }

    {
        auto blue = std::make_shared<DiffuseLight>(ConstantColor::Create(Spectrum(0.0f, 0.0f, light)));
        auto tf = Transform{ Point3(distance, 0.0f, 0.0f), Quat(pi, y_axis), Vec3(1.0f, size, size) };
        auto rect = CreateRectYZ(tf, blue);

        scene.AddLight(rect);
    }

    {
        auto white = std::make_shared<DiffuseLight>(ConstantColor::Create(Spectrum(0.5f)));

        auto tf = Transform{ Point3(0.0f, 4.0f, 0.0f), Quat(pi, x_axis), Vec3(8.0f, 1.0f, 8.0f) };
        auto rect = CreateRectXZ(tf, white);

        scene.AddLight(rect);
    }

    // {
    // auto white = std::make_shared<DiffuseLight>(SolidColor::Create(Spectrum(3.0f)));

    //     int32 count = 10;
    //     Float d = two_pi / count;

    //     Float y = 2.0f;
    //     Float r = 1.5f;

    //     for (int i = 0; i < count; ++i)
    //     {
    //         Float angle = d * i;
    //         auto pos = Vec3(std::cos(angle) * r, y, std::sin(angle) * r);
    //         auto sphere = std::make_shared<Sphere>(pos, 0.1f, white);

    //         scene.AddLight(sphere);
    //     }
    // }

    // Floor
    {
        auto mat = std::make_shared<Microfacet>(ConstantColor::Create(1.0f), ConstantColor::Create(Spectrum(0.0f)),
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

    // scene.AddLight(std::make_shared<InfiniteAreaLight>("res/solitude_night_4k/solitude_night_4k.hdr");
    // scene.AddLight(std::make_shared<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr");

    Float aspect_ratio = 16. / 9.;
    // Float aspect_ratio = 3. / 2.;
    // Float aspect_ratio = 4. / 3.;
    // Float aspect_ratio = 1.;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ 0, 0, 10 };
    Point3 lookat{ 0, 0, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 30;

    return std::make_unique<PerspectiveCamera>(lookfrom, lookat, y_axis, width, height, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("statue", StatueScene);

} // namespace bulbit