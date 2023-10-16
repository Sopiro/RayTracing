#include "spt/spt.h"

namespace spt
{

void RebootScene(Scene& scene)
{
    // https://sketchfab.com/3d-models/reboot-dramatic-scene-54ec601a3c4e4f6d8600fd28174c527c
    {
        auto mat = CreateSharedRef<Microfacet>(ConstantColor::Create(0.0), ConstantColor::Create(Spectrum(0.0f)),
                                               ConstantColor::Create(Spectrum(0.001f)));

        Material::fallback = mat;

        auto tf = Transform{ zero_vec3, Quat(DegToRad(0.0f), y_axis), Vec3(0.01f) };
        auto model = CreateSharedRef<Model>("res/reboot_dramatic_scene/scene.gltf", tf);

        scene.Add(model);
    }

    {
        auto red = CreateSharedRef<DiffuseLight>(Spectrum(14.0f, 0.0f, 0.0f));
        auto sphere = CreateSharedRef<Sphere>(Vec3(0.0f, 3.0f, -4.0f), 1.0f, red);

        scene.AddLight(sphere);
    }

    {
        auto white = CreateSharedRef<DiffuseLight>(Spectrum(8.0f));
        auto tf = Transform{ Vec3(0.0f, 8.0f, 0.0f), Quat(DegToRad(180.0f), x_axis), Vec3(3.0f) };
        auto rect = CreateRectXZ(tf, white);

        scene.AddLight(rect);
    }

    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/solitude_night_4k/solitude_night_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/photo_studio_01_1k.hdr"));
}

} // namespace spt