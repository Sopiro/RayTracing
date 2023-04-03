#include "raytracer/raytracer.h"

namespace spt
{

void GGXVNDFSamplingTest(Scene& scene)
{
    // Bunny
    {
        auto mat = RandomPBRMaterial();
        mat->basecolor_map = SolidColor::Create(Vec3{ 1.0 });
        mat->metallic_map = SolidColor::Create(Vec3{ 1.0 });
        mat->roughness_map = SolidColor::Create(Vec3{ 0.1 });

        // auto mat = std::make_shared<Dielectric>(1.5);

        Material::fallback_material = mat;

        auto tf = Transform{ identity };
        auto model = std::make_shared<Model>("res/stanford/bunny.obj", tf);

        scene.Add(model);
        // scene.AddLight(sphere);
    }

    scene.SetEnvironmentMap(ImageTexture::Create("res/sunflowers/sunflowers_4k.hdr", false, true));
}

} // namespace spt