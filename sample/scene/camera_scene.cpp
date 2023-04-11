#include "spt/pathtracer.h"

namespace spt
{

void CameraScene(Scene& scene)
{
    // Floor
    {
        auto mat = RandomPBRMaterial();
        mat->basecolor_map = SolidColor::Create(Vec3{ 0.5 });
        mat->metallic_map = SolidColor::Create(Vec3{ 0.0 });
        mat->roughness_map = SolidColor::Create(Vec3{ 0.01 });

        auto tf = Transform{ Vec3{ 0.0, 0.0, 0.0 }, Quat{ identity }, Vec3{ 8.0 } };
        auto floor = RectXZ(tf, mat);
        scene.Add(floor);
    }

    // Camera
    {
        auto tf = Transform{ Vec3{ 0.0, 0.0, 0.0 }, Quat{ DegToRad(0.0), y_axis }, Vec3{ 0.11 } };
        auto model = std::make_shared<Model>("res/AntiqueCamera/glTF/AntiqueCamera.gltf", tf);

        scene.Add(model);
    }

    // Lights
    {
        auto light = std::make_shared<DiffuseLight>(Color{ 1.0, 0.9, 0.8 } * 3.0);
        double w = 0.4;
        double h = 1.2;
        auto tf = Transform{ Point3{ 1.0, h / 2.0 - 0.01, 0.0 }, Quat{ pi, y_axis }, Vec3{ 1.0, h, w } };
        auto rect = RectYZ(tf, light);

        scene.Add(rect);
        scene.AddLight(rect);

        tf = Transform{ Point3{ 0.0, h / 2.0 - 0.01, -1.0 }, Quat{ 0.0, y_axis }, Vec3{ w, h, 1.0 } };
        rect = RectXY(tf, light);

        scene.Add(rect);
        scene.AddLight(rect);

        tf = Transform{ Point3{ 0.0, h / 2.0 - 0.01, 1.0 }, Quat{ pi, y_axis }, Vec3{ w, h, 1.0 } };
        rect = RectXY(tf, light);

        scene.Add(rect);
        scene.AddLight(rect);

        tf = Transform{ Point3{ -1.0, h / 2.0 - 0.01, 0.0 }, Quat{ 0.0, y_axis }, Vec3{ 1.0, h, w } };
        rect = RectYZ(tf, light);

        scene.Add(rect);
        scene.AddLight(rect);
    }

    // scene.SetEnvironmentMap(ImageTexture::Create("res/sunflowers/sunflowers_puresky_4k.hdr", false, true));
    scene.SetEnvironmentMap(SolidColor::Create(Color{ 0.0 }));
}

} // namespace spt