#include "spt/pathtracer.h"

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
        auto floor = CreateRectXZ(tf, mat, UV{ 4.0, 4.0 });
        scene.Add(floor);
    }

    f64 scale = 0.3;
    f64 gap = 0.2;

    // Bunny
    {
        auto tf = Transform{ Vec3{ gap * 3.0, 0.0, 0.0 }, Quat{ 0.0, y_axis }, Vec3{ scale } };
        auto mat = RandomPBRMaterial();
        Material::fallback_material = mat;

        auto model = CreateSharedRef<Model>("res/stanford/bunny.obj", tf);
        scene.Add(model);
    }

    // Lucy
    {
        auto tf = Transform{ Vec3{ gap, 0.0, 0.0 }, Quat{ 0.0, y_axis }, Vec3{ scale } };
        auto mat = RandomPBRMaterial();
        Material::fallback_material = mat;

        auto model = CreateSharedRef<Model>("res/stanford/lucy.obj", tf);
        scene.Add(model);
    }

    Srand(7777777);

    // Tyrannosaurus
    {
        auto tf = Transform{ Vec3{ -gap, 0.0, 0.0 }, Quat{ DegToRad(45.0), y_axis }, Vec3{ scale } };
        auto mat = RandomPBRMaterial();
        Material::fallback_material = mat;

        auto model = CreateSharedRef<Model>("res/stanford/tyra.obj", tf);
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

        auto model = CreateSharedRef<Model>("res/stanford/arma.obj", tf);
        scene.Add(model);
    }

    {
        f64 w = 0.04;
        f64 h = 0.6;
        f64 cx = 16.0;
        f64 xgap = 0.015;
        f64 xstep = 2.0 * w + xgap;

        auto light = CreateSharedRef<DiffuseLight>(Color{ 3.0 });
        light->two_sided = true;

        for (i32 x = 0; x < cx; ++x)
        {
            Vec3 pos;

            pos.y = 0.6;
            pos.x = x * xstep - ((cx - 1) * xstep / 2.0);
            pos.z = 0.0;

            auto mat = RandomPBRMaterial();

            auto tf = Transform{ pos, Quat{ pi, x_axis }, Vec3{ w, w, h } };
            auto rect = CreateRectXZ(tf, light);

            scene.Add(rect);
            scene.AddAreaLight(rect);
        }
    }

    // scene.SetEnvironmentMap(ImageTexture::Create("res/sunflowers/sunflowers_puresky_4k.hdr", false, true));
    scene.SetEnvironmentMap(SolidColor::Create(Color{ 0.0 }));
}

} // namespace spt