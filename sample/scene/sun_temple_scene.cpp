#include "../samples.h"

// https://developer.nvidia.com/ue4-sun-temple
std::unique_ptr<Camera> SunTempleScene(Scene& scene)
{
    Transform tf{ Vec3::zero, identity, Vec3(1.0f) };
    LoadModel(scene, "res/sun_temple/sun_temple.gltf", tf);

    // scene.CreateLight<ImageInfiniteLight>("res/HDR/SunTemple_Skybox.hdr"));

    Float intensity = 30;
    Spectrum color(1.0f, 0.392f, 0.122f);
    scene.CreateLight<PointLight>(Point3(-8.02094f, 3.98351f + 1, -2.05973f), intensity * color);
    scene.CreateLight<PointLight>(Point3(7.89041f, 3.98351f + 1, -2.14118f), intensity * color);
    scene.CreateLight<PointLight>(Point3(2.07289f, 3.9917f + 1, -7.75932f), intensity * color);
    scene.CreateLight<PointLight>(Point3(-3.09336f, 3.98351f + 1, 7.82017f), intensity * color);
    scene.CreateLight<PointLight>(Point3(3.15969f, 3.98351f + 1, 7.79511f), intensity * color);
    scene.CreateLight<PointLight>(Point3(7.46444f, 3.99169f + 1, 18.0761f), intensity * color);
    scene.CreateLight<PointLight>(Point3(2.41318f, 1.99327f, 28.5267f), intensity * color);
    scene.CreateLight<PointLight>(Point3(7.1504f, 1.99327f, 43.2531f), intensity * color);
    scene.CreateLight<PointLight>(Point3(0.030202f, 1.22295f, 31.5558f), intensity * color);

    Vec3 blender = Quat::FromEuler(DegToRad(-85.4428f), DegToRad(-30.6847f), DegToRad(57.7338f)).Rotate(Vec3(0, 0, -1));
    Vec3 dir(blender.x, blender.z, -blender.y);
    scene.CreateLight<DirectionalLight>(dir, 5 * Spectrum(1.0f, 0.569847f, 0.301f), 0.01f);

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 500;
    int32 height = int32(width / aspect_ratio);

    Point3 lookfrom{ -4.48045f, 9.22976f, -7.49469f };
    Point3 lookat{ 0, 8, 0 };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0;
    Float vFov = 54;

    return std::make_unique<PerspectiveCamera>(Vec2i(width, height), lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus);
}

static int32 index = Sample::Register("suntemple", SunTempleScene);
