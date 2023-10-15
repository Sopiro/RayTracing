#include "spt/spt.h"

namespace spt
{

void PBRTest(Scene& scene)
{
    // Srand(1234);

    // Spheres
    Float r = 0.3;
    Float cx = 10.0;
    Float cz = 7.0;
    Float xgap = 0.16;
    Float zgap = 0.14;
    Float xstep = 2.0 * r + xgap;
    Float zstep = 2.0 * r + zgap;

    for (int32 z = 0; z < cz; ++z)
    {
        for (int32 x = 0; x < cx; ++x)
        {
            Vec3 pos;

            pos.y = 0.0;
            pos.x = x * xstep - ((cx - 1) * xstep / 2.0);
            pos.z = z * zstep - ((cz - 1) * zstep / 2.0);

            auto mat = RandomMicrofacetMaterial();
            scene.Add(CreateSharedRef<Sphere>(pos, r, mat));
        }
    }

    // Ground
    {
        auto mat = CreateSharedRef<Microfacet>();
        mat->basecolor = ConstantColor::Create(Spectrum(1.0) * 0.9);
        mat->metallic = ConstantColor::Create(Spectrum(0.0));
        mat->roughness = ConstantColor::Create(Spectrum(0.1));
        mat->emissive = ConstantColor::Create(Spectrum(0.0));
        mat->normalmap = ConstantColor::Create(0.5, 0.5, 1.0);

        auto tf1 = Transform{ Vec3(0.5, -r, -0.5), identity, Vec3(100.0) };
        auto ground = CreateRectXZ(tf1, mat);

        scene.Add(ground);
    }

    // Light
    // {
    //     auto tf = Transform{ Vec3(-4.0, 2.5, 0.0), Quat(DegToRad(-40.0), z_axis), Vec3(1.0, 1.0, 4.0) };
    //     auto light = CreateSharedRef<DiffuseLight>(Spectrum(10.0));
    //     auto rect = RectYZ(tf, light);

    //     scene.Add(rect);
    //     scene.AddLight(rect);
    // }

    // {
    //     auto tf = Transform{ Vec3(4.0, 2.5, 0.0), Quat(DegToRad(180 + 50), z_axis), Vec3(1.0, 1.0, 4.0) };
    //     auto light = CreateSharedRef<DiffuseLight>(Spectrum(8.0));
    //     auto rect = RectYZ(tf, light);

    //     scene.Add(rect);
    //     scene.AddLight(rect);
    // }

    // {
    //     auto tf = Transform{ Vec3(0.0, 2.5, -4.0), Quat(DegToRad(40), x_axis), Vec3(4.0, 1.0, 1.0) };
    //     auto light = CreateSharedRef<DiffuseLight>(Spectrum(8.0));
    //     auto rect = RectXY(tf, light);

    //     scene.Add(rect);
    //     scene.AddLight(rect);
    // }

    {
        Float s = 0.4;
        Float cx = 10.0;
        Float xgap = 0.16;
        Float xstep = 2.0 * s + xgap;

        auto light = CreateSharedRef<DiffuseLight>(Spectrum(5.0));
        light->two_sided = true;

        for (int32 x = 0; x < cx; ++x)
        {
            Vec3 pos;

            pos.y = 2.2;
            pos.x = x * xstep - ((cx - 1) * xstep / 2.0);
            pos.z = 0.0;

            auto mat = RandomMicrofacetMaterial();

            auto tf = Transform{ pos, Quat(pi, x_axis), Vec3(s, s, 2.0) };
            auto rect = CreateRectXZ(tf, light);

            scene.AddLight(rect);
        }
    }

    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/sunset.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/peppermint_powerplant_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/HDR/kloppenheim_07_puresky_1k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/sunflowers/sunflowers_puresky_4k.hdr"));
    // scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/solitude_night_4k/solitude_night_4k.hdr"));
}

} // namespace spt