#include "spt/spt.h"

namespace spt
{

void GGXVNDFSamplingTest(Scene& scene)
{
    // Bunny
    {
        auto mat = RandomMicrofacetMaterial();
        mat->basecolor = ConstantColor::Create(Spectrum(1.0f));
        mat->metallic = ConstantColor::Create(Spectrum(1.0f));
        mat->roughness = ConstantColor::Create(Spectrum(0.1f));

        // auto mat = CreateSharedRef<Dielectric>(1.5f);

        Material::fallback = mat;

        auto tf = Transform{ zero_vec3, Quat(DegToRad(0.0f), y_axis), Vec3(3.0f) };
        auto model = CreateSharedRef<Model>("res/stanford/bunny.obj", tf);

        scene.Add(model);
        // scene.AddLight(sphere);
    }

    scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/scythian_tombs_2_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/quarry_04_puresky_1k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/solitude_night_4k/solitude_night_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr"));
}

} // namespace spt