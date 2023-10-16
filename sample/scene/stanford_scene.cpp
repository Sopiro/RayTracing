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
        mat->normalmap = ImageTexture::Create("res/dark_wooden_planks_4k/textures/dark_wooden_planks_nor_gl_4k.png");

        auto tf = Transform{ zero_vec3, identity, Vec3(8.0f) };
        auto floor = CreateRectXZ(tf, mat, Point2(4.0f, 4.0f));
        scene.Add(floor);
    }

    Float scale = 0.3f;
    Float gap = 0.2f;

    // Bunny
    {
        auto tf = Transform{ Vec3(gap * 3.0f, 0.0f, 0.0f), Quat(0.0f, y_axis), Vec3(scale) };
        auto mat = RandomMicrofacetMaterial();
        Material::fallback = mat;

        auto model = CreateSharedRef<Model>("res/stanford/bunny.obj", tf);
        scene.Add(model);
    }

    // Lucy
    {
        auto tf = Transform{ Vec3(gap, 0.0f, 0.0f), Quat(0.0f, y_axis), Vec3(scale) };
        auto mat = RandomMicrofacetMaterial();
        Material::fallback = mat;

        auto model = CreateSharedRef<Model>("res/stanford/lucy.obj", tf);
        scene.Add(model);
    }

    Srand(7777777);

    // Tyrannosaurus
    {
        auto tf = Transform{ Vec3(-gap, 0.0f, 0.0f), Quat(DegToRad(45.0f), y_axis), Vec3(scale) };
        auto mat = RandomMicrofacetMaterial();
        Material::fallback = mat;

        auto model = CreateSharedRef<Model>("res/stanford/tyra.obj", tf);
        scene.Add(model);
    }

    Srand(7654321);

    // Armadillo
    {
        auto tf = Transform{ Vec3(-gap * 3.0f, 0.0f, 0.0f), Quat(0.0f, y_axis), Vec3(scale) };
        auto mat = RandomMicrofacetMaterial();
        mat->metallic = ConstantColor::Create(Spectrum(1.0f));
        mat->roughness = ConstantColor::Create(Spectrum(0.2f));
        Material::fallback = mat;

        auto model = CreateSharedRef<Model>("res/stanford/arma.obj", tf);
        scene.Add(model);
    }

    {
        Float w = 0.04f;
        Float h = 0.6f;
        Float cx = 16.0f;
        Float xgap = 0.015f;
        Float xstep = 2.0f * w + xgap;

        auto light = CreateSharedRef<DiffuseLight>(Spectrum(3.0f));
        light->two_sided = true;

        for (int32 x = 0; x < cx; ++x)
        {
            Vec3 pos;

            pos.y = 0.6f;
            pos.x = x * xstep - ((cx - 1) * xstep / 2.0f);
            pos.z = 0.0f;

            auto mat = RandomMicrofacetMaterial();

            auto tf = Transform{ pos, Quat(pi, x_axis), Vec3(w, w, h) };
            auto rect = CreateRectXZ(tf, light);

            scene.AddLight(rect);
        }
    }

    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr"));
}

} // namespace spt