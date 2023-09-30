#include "spt/spt.h"

namespace spt
{

void RebootScene(Scene& scene)
{
    // https://sketchfab.com/3d-models/reboot-dramatic-scene-54ec601a3c4e4f6d8600fd28174c527c
    {
        auto mat = RandomMicrofacetMaterial();
        mat->basecolor = SolidColor::Create(Vec3(0.0));
        mat->metallic = SolidColor::Create(Vec3(0.0));
        mat->roughness = SolidColor::Create(Vec3(0.001));

        Material::fallback = mat;

        auto tf = Transform{ zero_vec3, Quat(DegToRad(0.0), y_axis), Vec3(0.01) };
        auto model = CreateSharedRef<Model>("res/reboot_dramatic_scene/scene.gltf", tf);

        scene.Add(model);
    }

    {
        auto red = CreateSharedRef<DiffuseLight>(Color(14.0, 0.0, 0.0));
        auto sphere = CreateSharedRef<Sphere>(Vec3(0.0, 3.0, -4.0), 1.0, red);

        scene.AddLight(sphere);
    }

    {
        auto white = CreateSharedRef<DiffuseLight>(Color(8.0));
        auto tf = Transform{ Vec3(0.0, 8.0, 0.0), Quat(DegToRad(180.0), x_axis), Vec3(3.0) };
        auto rect = CreateRectXZ(tf, white);

        scene.AddLight(rect);
    }

    scene.AddLight(
        CreateSharedRef<InfiniteAreaLight>(ImageTexture::Create("res/solitude_night_4k/solitude_night_4k.hdr", false, true)));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>(ImageTexture::Create("res/HDR/photo_studio_01_1k.hdr", false, true)));
}

} // namespace spt