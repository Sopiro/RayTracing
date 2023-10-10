#include "spt/spt.h"

namespace spt
{

void StanfordScene(Scene& scene)
{
    // Floor
    {
        auto mat = RandomMicrofacetMaterial();
        mat->basecolor = ImageTexture::Create("res/dark_wooden_planks_4k/textures/dark_wooden_planks_diff_4k.jpg");
        mat->metallic = ImageTexture::Create("res/dark_wooden_planks_4k/textures/dark_wooden_planks_arm_4k.jpg");
        mat->roughness = ImageTexture::Create("res/dark_wooden_planks_4k/textures/dark_wooden_planks_arm_4k.jpg");
        mat->normal_map = ImageTexture::Create("res/dark_wooden_planks_4k/textures/dark_wooden_planks_nor_gl_4k.png");

        auto tf = Transform{ zero_vec3, Quat(identity), Vec3(8.0) };
        auto floor = CreateRectXZ(tf, mat, Point2(4.0, 4.0));
        scene.Add(floor);
    }

    Float scale = 0.3;
    Float gap = 0.2;

    // Bunny
    {
        auto tf = Transform{ Vec3(gap * 3.0, 0.0, 0.0), Quat(0.0, y_axis), Vec3(scale) };
        auto mat = RandomMicrofacetMaterial();
        Material::fallback = mat;

        auto model = CreateSharedRef<Model>("res/stanford/bunny.obj", tf);
        scene.Add(model);
    }

    // Lucy
    {
        auto tf = Transform{ Vec3(gap, 0.0, 0.0), Quat(0.0, y_axis), Vec3(scale) };
        auto mat = RandomMicrofacetMaterial();
        Material::fallback = mat;

        auto model = CreateSharedRef<Model>("res/stanford/lucy.obj", tf);
        scene.Add(model);
    }

    Srand(7777777);

    // Tyrannosaurus
    {
        auto tf = Transform{ Vec3(-gap, 0.0, 0.0), Quat(DegToRad(45.0), y_axis), Vec3(scale) };
        auto mat = RandomMicrofacetMaterial();
        Material::fallback = mat;

        auto model = CreateSharedRef<Model>("res/stanford/tyra.obj", tf);
        scene.Add(model);
    }

    Srand(7654321);

    // Armadillo
    {
        auto tf = Transform{ Vec3(-gap * 3.0, 0.0, 0.0), Quat(0.0, y_axis), Vec3(scale) };
        auto mat = RandomMicrofacetMaterial();
        mat->metallic = ConstantColor::Create(Color(1.0));
        mat->roughness = ConstantColor::Create(Color(0.2));
        Material::fallback = mat;

        auto model = CreateSharedRef<Model>("res/stanford/arma.obj", tf);
        scene.Add(model);
    }

    {
        Float w = 0.04;
        Float h = 0.6;
        Float cx = 16.0;
        Float xgap = 0.015;
        Float xstep = 2.0 * w + xgap;

        auto light = CreateSharedRef<DiffuseLight>(Color(3.0));
        light->two_sided = true;

        for (int32 x = 0; x < cx; ++x)
        {
            Vec3 pos;

            pos.y = 0.6;
            pos.x = x * xstep - ((cx - 1) * xstep / 2.0);
            pos.z = 0.0;

            auto mat = RandomMicrofacetMaterial();

            auto tf = Transform{ pos, Quat(pi, x_axis), Vec3(w, w, h) };
            auto rect = CreateRectXZ(tf, light);

            scene.AddLight(rect);
        }
    }

    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr"));
}

} // namespace spt