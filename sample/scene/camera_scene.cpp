#include "spt/spt.h"

namespace spt
{

void CameraScene(Scene& scene)
{
    // Floor
    {
        auto mat = RandomMicrofacetMaterial();
        mat->basecolor = SolidColor::Create(Vec3(0.5));
        mat->metallic = SolidColor::Create(Vec3(0.0));
        mat->roughness = SolidColor::Create(Vec3(0.01));

        auto tf = Transform{ zero_vec3, Quat(identity), Vec3(8.0) };
        auto floor = CreateRectXZ(tf, mat);
        scene.Add(floor);
    }

    // Camera
    {
        auto tf = Transform{ zero_vec3, Quat(DegToRad(0.0), y_axis), Vec3(0.11) };
        auto model = CreateSharedRef<Model>("res/AntiqueCamera/glTF/AntiqueCamera.gltf", tf);

        scene.Add(model);
    }

    // Lights
    {
        auto light = CreateSharedRef<DiffuseLight>(Color(1.0, 0.9, 0.8) * 3.0);
        Float w = 0.4;
        Float h = 1.2;
        auto tf = Transform{ Point3(1.0, h / 2.0 - 0.01, 0.0), Quat(pi, y_axis), Vec3(1.0, h, w) };
        auto rect = CreateRectYZ(tf, light);

        scene.AddLight(rect);

        tf = Transform{ Point3(0.0, h / 2.0 - 0.01, -1.0), Quat(0.0, y_axis), Vec3(w, h, 1.0) };
        rect = CreateRectXY(tf, light);

        scene.AddLight(rect);

        tf = Transform{ Point3(0.0, h / 2.0 - 0.01, 1.0), Quat(pi, y_axis), Vec3(w, h, 1.0) };
        rect = CreateRectXY(tf, light);

        scene.AddLight(rect);

        tf = Transform{ Point3(-1.0, h / 2.0 - 0.01, 0.0), Quat(0.0, y_axis), Vec3(1.0, h, w) };
        rect = CreateRectYZ(tf, light);

        scene.AddLight(rect);
    }

    // scene.SetEnvironmentMap(ImageTexture::Create("res/sunflowers/sunflowers_puresky_4k.hdr", false, true));
    scene.AddLight(CreateSharedRef<InfiniteAreaLight>(SolidColor::Create(Color(0.0))));
}

} // namespace spt