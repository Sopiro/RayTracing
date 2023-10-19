#include "bulbit/diffuse_light.h"
#include "bulbit/lambertian.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

// https://developer.nvidia.com/ue4-sun-temple
void SunTempleScene(Scene& scene)
{
    Transform tf{ zero_vec3, identity, Vec3(1.0f) };
    Ref<Model> m = CreateSharedRef<Model>("res/sun_temple/sun_temple.gltf", tf);
    scene.Add(m);

    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/SunTemple_Skybox.hdr"));

    Float intensity = 30;
    Spectrum color(1.0f, 0.392f, 0.122f);
    scene.AddLight(CreateSharedRef<PointLight>(Point3(-8.02094f, 3.98351f + 1, -2.05973f), intensity * color));
    scene.AddLight(CreateSharedRef<PointLight>(Point3(7.89041f, 3.98351f + 1, -2.14118f), intensity * color));
    scene.AddLight(CreateSharedRef<PointLight>(Point3(2.07289f, 3.9917f + 1, -7.75932f), intensity * color));
    scene.AddLight(CreateSharedRef<PointLight>(Point3(-3.09336f, 3.98351f + 1, 7.82017f), intensity * color));
    scene.AddLight(CreateSharedRef<PointLight>(Point3(3.15969f, 3.98351f + 1, 7.79511f), intensity * color));
    scene.AddLight(CreateSharedRef<PointLight>(Point3(7.46444f, 3.99169f + 1, 18.0761f), intensity * color));
    scene.AddLight(CreateSharedRef<PointLight>(Point3(2.41318f, 1.99327f, 28.5267f), intensity * color));
    scene.AddLight(CreateSharedRef<PointLight>(Point3(7.1504f, 1.99327f, 43.2531f), intensity * color));
    scene.AddLight(CreateSharedRef<PointLight>(Point3(0.030202f, 1.22295f, 31.5558f), intensity * color));

    Vec3 blender = Quat::FromEuler(DegToRad(-85.4428f), DegToRad(-30.6847f), DegToRad(57.7338f)) * Vec3(0, 0, -1);
    Vec3 dir(blender.x, blender.z, -blender.y);
    scene.AddLight(CreateSharedRef<DirectionalLight>(dir, 5 * Spectrum(1.0f, 0.569847f, 0.301f), 0.01f));
}

} // namespace bulbit