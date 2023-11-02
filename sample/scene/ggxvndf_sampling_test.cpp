#include "../samples.h"
#include "bulbit/diffuse_light.h"
#include "bulbit/lambertian.h"
#include "bulbit/perspective_camera.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/util.h"

namespace bulbit
{

Camera* GGXVNDFSamplingTest(Scene& scene)
{
    // Bunny
    {
        auto mat = CreateSharedRef<Microfacet>(ConstantColor::Create(1.0f), ConstantColor::Create(Spectrum(1.0f)),
                                               ConstantColor::Create(Spectrum(0.1f)));

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

    Float aspect_ratio = 16.0f / 9.0f;

    Point3 lookfrom{ 0, 2, 10 };
    Point3 lookat{ 0, 1, 0 };

    Float dist_to_focus = (lookfrom - lookat).Length();
    Float aperture = 0;
    Float vFov = 30;

    return new PerspectiveCamera(lookfrom, lookat, y_axis, vFov, aspect_ratio, aperture, dist_to_focus);
}

static int32 index = Sample::Register("ggxvndf", GGXVNDFSamplingTest);

} // namespace bulbit