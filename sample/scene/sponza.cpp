#include "../samples.h"

std::unique_ptr<Camera> Sponza(Scene& scene)
{
    // Transform transform{ Vec3::zero, Quat(DegToRad(90.0f), y_axis), Vec3(0.01f) };
    // LoadModel(scene, "res/sponza2/sponza.obj", transform);

    Transform transform{ Vec3::zero, Quat(DegToRad(90.0f), y_axis), Vec3(1.0f) };
    LoadModel(scene, "res/sponza/glTF/Sponza.gltf", transform);

    // auto light = scene.CreateMaterial<DiffuseLight>(Spectrum(1.0f));
    // auto mat = scene.CreateMaterial<Dielectric>(1.5f);

    Float cx = 8.0f;
    Float cy = 4.0f;
    Float cz = 8.0f;

    Float sx = 10.0f;
    Float sy = 10.0f;
    Float sz = 20.0f;

    Float xm = -sx / 2.0f;
    Float ym = 0.0f;
    Float zm = -sz / 2.0f;

    // for (int32 z = 0; z < cz; ++z)
    // {
    //     for (int32 y = 0; y < cy; ++y)
    //     {
    //         for (int32 x = 0; x < cx; ++x)
    //         {
    //             auto sphere = std::make_shared<Sphere>(Vec3(x / cx * sx + xm, y / cy * sy + ym, z / cz * sz + zm), 0.1, light);
    //             scene.Add(sphere);
    //             scene.AddLight(sphere);
    //         }
    //     }
    // }

    // {
    //     auto light2 = scene.CreateMaterial<DiffuseLight>(Spectrum(20.0f));
    //     auto sphere = std::make_shared<Sphere>(Vec3(0.0f, 1.5f, 0.0f), 0.4f, light2);
    //     scene.Add(sphere);
    //     scene.AddLight(sphere);
    // }

    scene.CreateLight<ImageInfiniteLight>("res/HDR/quarry_04_puresky_1k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/sunflowers/sunflowers_puresky_4k.hdr");
    // scene.CreateLight<ImageInfiniteLight>("res/solitude_night_4k/solitude_night_4k.hdr");

    Spectrum sky_color(147 / 255.0f, 209 / 255.0f, 255 / 255.0f);

    scene.CreateLight<DirectionalLight>(Normalize(-Vec3(-3.0f, 15.0f, -3.0f)), Vec3(15.0f), 0.02f);
    // scene.CreateLight<UniformInfiniteLight>(sky_color, 15.0f);

    Float aspect_ratio = 16.f / 9.f;
    // Float aspect_ratio = 3.f / 2.f;
    // Float aspect_ratio = 4.f / 3.f;
    // Float aspect_ratio = 1.f;
    int32 width = 960;
    int32 height = int32(width / aspect_ratio);

    // Point3 lookfrom{ 0.0f, 2.5f, 4.5f };
    // Point3 lookat{ 0.0f, 1.45f, 0.0f };

    // Point3 lookfrom{ -1.5f, 5.f5, 10.0f };
    // Point3 lookat{ 0.0f, 3.45f, 0.0f };

    // Point3 lookfrom{ 0.0f, 0.5f, 7.0f };
    // Point3 lookat{ 0.0f, 3.0f, 0.0f };

    // Point3 lookfrom{ -4, 4.5, 6 };
    // Point3 lookat{ -4, 4.5, 0 };

    // Point3 lookfrom{ 0, 1, 6 };
    // Point3 lookat{ 0, 1, 0 };

    Point3 lookfrom{ 0, 5, 6 };
    Point3 lookat{ 0, 5, 0 };

    Float dist_to_focus = Dist(lookfrom, lookat);
    Float aperture = 0;
    Float vFov = 71;

    return std::make_unique<PerspectiveCamera>(Point2i(width, height), lookfrom, lookat, y_axis, vFov, aperture, dist_to_focus);
    // return std::make_unique<SphericalCamera>(lookfrom, Point2i(width, height));
}

static int32 index = Sample::Register("sponza", Sponza);
