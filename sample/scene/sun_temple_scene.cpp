#include "spt/spt.h"

namespace spt
{

// https://developer.nvidia.com/ue4-sun-temple
void SunTempleScene(Scene& scene)
{
    Transform tf{ zero_vec3, identity, Vec3(1.0) };
    Ref<Model> m = CreateSharedRef<Model>("res/sun_temple/sun_temple.gltf", tf);
    scene.Add(m);

    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/SunTemple_Skybox.hdr"));

    Float intensity = 30;
    scene.AddLight(CreateSharedRef<PointLight>(Point3(-8.02094, 3.98351 + 1, -2.05973), intensity * Spectrum(1, 0.392, 0.122)));
    scene.AddLight(CreateSharedRef<PointLight>(Point3(7.89041, 3.98351 + 1, -2.14118), intensity * Spectrum(1, 0.392, 0.122)));
    scene.AddLight(CreateSharedRef<PointLight>(Point3(2.07289, 3.9917 + 1, -7.75932), intensity * Spectrum(1, 0.392, 0.122)));
    scene.AddLight(CreateSharedRef<PointLight>(Point3(-3.09336, 3.98351 + 1, 7.82017), intensity * Spectrum(1, 0.392, 0.122)));
    scene.AddLight(CreateSharedRef<PointLight>(Point3(3.15969, 3.98351 + 1, 7.79511), intensity * Spectrum(1, 0.392, 0.122)));
    scene.AddLight(CreateSharedRef<PointLight>(Point3(7.46444, 3.99169 + 1, 18.0761), intensity * Spectrum(1, 0.392, 0.122)));
    scene.AddLight(CreateSharedRef<PointLight>(Point3(2.41318, 1.99327, 28.5267), intensity * Spectrum(1, 0.392, 0.122)));
    scene.AddLight(CreateSharedRef<PointLight>(Point3(7.1504, 1.99327, 43.2531), intensity * Spectrum(1, 0.392, 0.122)));
    scene.AddLight(CreateSharedRef<PointLight>(Point3(0.030202, 1.22295, 31.5558), intensity * Spectrum(1, 0.392, 0.122)));

    Vec3 blender = Quat::FromEuler(DegToRad(-85.4428), DegToRad(-30.6847), DegToRad(57.7338)) * Vec3(0, 0, -1);
    Vec3 dir(blender.x, blender.z, -blender.y);
    scene.AddLight(CreateSharedRef<DirectionalLight>(dir, 5 * Spectrum(1, 0.569847, 0.301), 0.01));
}

} // namespace spt