#include "raytracer/raytracer.h"

namespace spt
{

void StanfordScene(Scene& scene)
{
    // Floor
    {
        auto mat = RandomPBRMaterial();
        mat->basecolor_map = ImageTexture::Create("res/dark_wooden_planks_4k/textures/dark_wooden_planks_diff_4k.jpg");
        mat->normal_map = ImageTexture::Create("res/dark_wooden_planks_4k/textures/dark_wooden_planks_nor_gl_4k.png");
        mat->ao_map = ImageTexture::Create("res/dark_wooden_planks_4k/textures/dark_wooden_planks_arm_4k.jpg");
        mat->roughness_map = ImageTexture::Create("res/dark_wooden_planks_4k/textures/dark_wooden_planks_arm_4k.jpg");
        mat->metallic_map = ImageTexture::Create("res/dark_wooden_planks_4k/textures/dark_wooden_planks_arm_4k.jpg");

        auto tf = Transform{ Vec3{ 0.0, 0.0, 0.0 }, Quat{ identity }, Vec3{ 8.0 } };
        auto floor = RectXZ(tf, mat, UV{ 4.0, 4.0 });
        scene.Add(floor);
    }

    double scale = 0.3;
    double gap = 0.2;

    // Bunny
    {
        auto tf = Transform{ Vec3{ gap * 3.0, 0.0, 0.0 }, Quat{ 0.0, y_axis }, Vec3{ scale } };
        auto mat = RandomPBRMaterial();
        Material::fallback_material = mat;

        auto model = std::make_shared<Model>("res/stanford/bunny.obj", tf);
        scene.Add(model);
    }

    // Lucy
    {
        auto tf = Transform{ Vec3{ gap, 0.0, 0.0 }, Quat{ 0.0, y_axis }, Vec3{ scale } };
        auto mat = RandomPBRMaterial();
        Material::fallback_material = mat;

        auto model = std::make_shared<Model>("res/stanford/lucy.obj", tf);
        scene.Add(model);
    }

    Srand(7777777);

    // Tyrannosaurus
    {
        auto tf = Transform{ Vec3{ -gap, 0.0, 0.0 }, Quat{ DegToRad(45.0), y_axis }, Vec3{ scale } };
        auto mat = RandomPBRMaterial();
        Material::fallback_material = mat;

        auto model = std::make_shared<Model>("res/stanford/tyra.obj", tf);
        scene.Add(model);
    }

    Srand(7654321);

    // Armadillo
    {
        auto tf = Transform{ Vec3{ -gap * 3.0, 0.0, 0.0 }, Quat{ 0.0, y_axis }, Vec3{ scale } };
        auto mat = RandomPBRMaterial();
        mat->metallic_map = SolidColor::Create(Color{ 1.0 });
        mat->roughness_map = SolidColor::Create(Color{ 0.2 });
        Material::fallback_material = mat;

        auto model = std::make_shared<Model>("res/stanford/arma.obj", tf);
        scene.Add(model);
    }

    {
        double w = 0.04;
        double h = 0.6;
        double cx = 16.0;
        double xgap = 0.015;
        double xstep = 2.0 * w + xgap;

        auto light = std::make_shared<DiffuseLight>(Color{ 3.0 });
        light->two_sided = true;

        for (int32 x = 0; x < cx; ++x)
        {
            Vec3 pos;

            pos.y = 0.6;
            pos.x = x * xstep - ((cx - 1) * xstep / 2.0);
            pos.z = 0.0;

            auto mat = RandomPBRMaterial();

            auto tf = Transform{ pos, Quat{ pi, x_axis }, Vec3{ w, w, h } };
            auto rect = RectXZ(tf, light);

            scene.Add(rect);
            scene.AddLight(rect);
        }
    }

    // scene.SetEnvironmentMap(ImageTexture::Create("res/sunflowers/sunflowers_puresky_4k.hdr", false, true));
    scene.SetEnvironmentMap(SolidColor::Create(Color{ 0.0 }));
}

} // namespace spt