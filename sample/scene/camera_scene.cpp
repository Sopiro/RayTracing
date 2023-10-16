#include "spt/spt.h"

namespace spt
{

void CameraScene(Scene& scene)
{
    // Floor
    {
        auto mat = RandomMicrofacetMaterial();
        mat->basecolor = ConstantColor::Create(Spectrum(0.5f));
        mat->metallic = ConstantColor::Create(Spectrum(0.0f));
        mat->roughness = ConstantColor::Create(Spectrum(0.01f));

        auto tf = Transform{ zero_vec3, identity, Vec3(8.0f) };
        auto floor = CreateRectXZ(tf, mat);
        scene.Add(floor);
    }

    // Camera
    {
        auto tf = Transform{ zero_vec3, Quat(DegToRad(0.0f), y_axis), Vec3(0.11f) };
        auto model = CreateSharedRef<Model>("res/AntiqueCamera/glTF/AntiqueCamera.gltf", tf);

        scene.Add(model);
    }

    // Lights
    {
        auto light = CreateSharedRef<DiffuseLight>(Spectrum(1.0f, 0.9f, 0.8f) * 3);
        Float w = 0.4f;
        Float h = 1.2f;
        auto tf = Transform{ Point3(1.0f, h / 2.0f - 0.01f, 0.0f), Quat(pi, y_axis), Vec3(1.0f, h, w) };
        auto rect = CreateRectYZ(tf, light);

        scene.AddLight(rect);

        tf = Transform{ Point3(0.0f, h / 2.0f - 0.01f, -1.0f), Quat(0.0f, y_axis), Vec3(w, h, 1.0f) };
        rect = CreateRectXY(tf, light);

        scene.AddLight(rect);

        tf = Transform{ Point3(0.0f, h / 2.0f - 0.01f, 1.0f), Quat(pi, y_axis), Vec3(w, h, 1.0f) };
        rect = CreateRectXY(tf, light);

        scene.AddLight(rect);

        tf = Transform{ Point3(-1.0f, h / 2.0f - 0.01f, 0.0f), Quat(0.0f, y_axis), Vec3(1.0f, h, w) };
        rect = CreateRectYZ(tf, light);

        scene.AddLight(rect);
    }

    // scene.SetEnvironmentMap(ImageTexture::Create("res/sunflowers/sunflowers_puresky_4k.hdr", false, true));
}

} // namespace spt