#include "spt/spt.h"

namespace spt
{

void GGXVNDFSamplingTest(Scene& scene)
{
    // Bunny
    {
        auto mat = RandomMicrofacetMaterial();
        mat->basecolor = ConstantColor::Create(Vec3(1.0));
        mat->metallic = ConstantColor::Create(Vec3(1.0));
        mat->roughness = ConstantColor::Create(Vec3(0.1));

        // auto mat = CreateSharedRef<Dielectric>(1.5);

        Material::fallback = mat;

        auto tf = Transform{ zero_vec3, Quat(DegToRad(0.0), y_axis), Vec3(3.0) };
        auto model = CreateSharedRef<Model>("res/stanford/bunny.obj", tf);

        scene.Add(model);
        // scene.AddLight(sphere);
    }

    scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/scythian_tombs_2_4k.hdr", false, true));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/quarry_04_puresky_1k.hdr", false, true));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/solitude_night_4k/solitude_night_4k.hdr", false, true));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr", false, true));
}

} // namespace spt