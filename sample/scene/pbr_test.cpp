#include "spt/spt.h"

namespace spt
{

void PBRTest(Scene& scene)
{
    // Srand(1234);

    // Spheres
    Float r = 0.3f;
    Float cx = 10.0f;
    Float cz = 7.0f;
    Float xgap = 0.16f;
    Float zgap = 0.14f;
    Float xstep = 2.0f * r + xgap;
    Float zstep = 2.0f * r + zgap;

    for (int32 z = 0; z < cz; ++z)
    {
        for (int32 x = 0; x < cx; ++x)
        {
            Vec3 pos;

            pos.y = 0;
            pos.x = x * xstep - ((cx - 1) * xstep / 2);
            pos.z = z * zstep - ((cz - 1) * zstep / 2);

            auto mat = RandomMicrofacetMaterial();
            scene.Add(CreateSharedRef<Sphere>(pos, r, mat));
        }
    }

    // Ground
    {
        auto mat = CreateSharedRef<Microfacet>();
        mat->basecolor = ConstantColor::Create(Spectrum(1.0f) * 0.9f);
        mat->metallic = ConstantColor::Create(Spectrum(0.0f));
        mat->roughness = ConstantColor::Create(Spectrum(0.1f));
        mat->emissive = ConstantColor::Create(Spectrum(0.0));
        mat->normalmap = ConstantColor::Create(0.5f, 0.5f, 1.0f);

        auto tf1 = Transform{ Vec3(0.5f, -r, -0.5f), identity, Vec3(100.0f) };
        auto ground = CreateRectXZ(tf1, mat);

        scene.Add(ground);
    }

    // Light
    // {
    //     auto tf = Transform{ Vec3(-4.0f, 2.5f, 0.0f), Quat(DegToRad(-40.0f), z_axis), Vec3(1.0f, 1.0f, 4.0f) };
    //     auto light = CreateSharedRef<DiffuseLight>(Spectrum(10.0f));
    //     auto rect = RectYZ(tf, light);

    //     scene.Add(rect);
    //     scene.AddLight(rect);
    // }

    // {
    //     auto tf = Transform{ Vec3(4.0f, 2.5f, 0.0f), Quat(DegToRad(180 + 50), z_axis), Vec3(1.0f, 1.0f, 4.0f) };
    //     auto light = CreateSharedRef<DiffuseLight>(Spectrum(8.0f));
    //     auto rect = RectYZ(tf, light);

    //     scene.Add(rect);
    //     scene.AddLight(rect);
    // }

    // {
    //     auto tf = Transform{ Vec3(0.0f, 2.5f, -4.0f), Quat(DegToRad(40), x_axis), Vec3(4.0f, 1.0f, 1.0f) };
    //     auto light = CreateSharedRef<DiffuseLight>(Spectrum(8.0f));
    //     auto rect = RectXY(tf, light);

    //     scene.Add(rect);
    //     scene.AddLight(rect);
    // }

    {
        Float s = 0.4f;
        Float cx = 10.0f;
        Float xgap = 0.16f;
        Float xstep = 2.0f * s + xgap;

        auto light = CreateSharedRef<DiffuseLight>(Spectrum(5.0f));
        light->two_sided = true;

        for (int32 x = 0; x < cx; ++x)
        {
            Vec3 pos;

            pos.y = 2.2f;
            pos.x = x * xstep - ((cx - 1) * xstep / 2);
            pos.z = 0.0f;

            auto mat = RandomMicrofacetMaterial();

            auto tf = Transform{ pos, Quat(pi, x_axis), Vec3(s, s, 2) };
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