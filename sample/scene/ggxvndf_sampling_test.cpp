#include "spt/pathtracer.h"

namespace spt
{

void GGXVNDFSamplingTest(Scene& scene)
{
    // Bunny
    {
        auto mat = RandomMicrofacetMaterial();
        mat->basecolor = SolidColor::Create(Vec3(1.0));
        mat->metallic = SolidColor::Create(Vec3(1.0));
        mat->roughness = SolidColor::Create(Vec3(0.1));

        // auto mat = CreateSharedRef<Dielectric>(1.5);

        Material::fallback = mat;

        auto tf = Transform{ zero_vec3, Quat(DegToRad(0.0), y_axis), Vec3(3.0) };
        auto model = CreateSharedRef<Model>("res/stanford/bunny.obj", tf);

        scene.Add(model);
        // scene.AddLight(sphere);
    }

    scene.SetEnvironmentMap(ImageTexture::Create("res/solitude_night_4k/solitude_night_4k.hdr", false, true));
    // scene.SetEnvironmentMap(ImageTexture::Create("res/sunflowers/sunflowers_puresky_4k.hdr", false, true));
}

} // namespace spt