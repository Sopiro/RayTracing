#include "spt/spt.h"
#include "spt/util.h"

namespace spt
{

void EnvironmentMap(Scene& scene)
{
    Float r = 1.0f;
    Float cx = 4.0f;
    Float cz = 1.0f;
    Float xgap = 0.16f;
    Float zgap = 0.14f;
    Float xstep = 2.0f * r + xgap;
    Float zstep = 2.0f * r + zgap;

    for (int32 z = 0; z < cz; ++z)
    {
        for (int32 x = 0; x < cx; ++x)
        {
            Vec3 pos;

            pos.y = 0.0f;
            pos.x = x * xstep - ((cx - 1) * xstep / 2);
            pos.z = z * zstep - ((cz - 1) * zstep / 2);

            // auto mat = RandomMaterial();

            Ref<Material> mat;
            if (x >= 2)
            {
                mat = CreateSharedRef<Dielectric>(1.5f);
            }
            else
            {
                mat = CreateSharedRef<Metal>(Vec3(0.6f), 0.0f);
            }

            if (x % 2)
            {
                scene.Add(CreateSharedRef<Sphere>(pos, r, mat));
            }
            else
            {
                auto tf = Transform{ pos, identity, Vec3(r * 1.414f) };
                scene.Add(CreateBox(tf, mat));
            }
        }
    }

    // Ground
    // {
    //     auto mat = CreateSharedRef<PBRMaterial>();
    //     mat->albedo_map = ConstantColor::Create(Spectrum(1.0f) * 0.9f);
    //     mat->normal_map = ConstantColor::Create(0.5f, 0.5f, 1.0f);
    //     mat->roughness_map = ConstantColor::Create(Spectrum(0.1f));
    //     mat->metallic_map = ConstantColor::Create(Spectrum(0.0f));
    //     mat->ao_map = ConstantColor::Create(Spectrum(1.0f));
    //     mat->emissive_map = ConstantColor::Create(Spectrum(0.0f));

    //     auto tf1 = Transform{ Vec3(0.5f, -r, -0.5f), identity, Vec3(100.0f) };
    //     auto ground = RectXZ(tf1, mat);

    //     scene.Add(ground);
    // }

    // // Light
    // {
    //     auto tf2 = Transform{ Vec3(-4.0f, 2.5f, 0.0f), Quat(DegToRad(-40.0f), z_axis), Vec3(1.0f, 1.0f, 4.0f) };
    //     auto light = CreateSharedRef<DiffuseLight>(Spectrum(8.0));
    //     auto rect = RectYZ(tf2, light);

    //     scene.Add(rect);
    //     scene.AddLight(rect);
    // }

    // {
    //     auto tf2 = Transform{ Vec3(4.0f, 2.5f, 0.0f), Quat(DegToRad(180 + 50), z_axis), Vec3(1.0f, 1.0f, 4.0f) };
    //     auto light = CreateSharedRef<DiffuseLight>(Spectrum(8.0));
    //     auto rect = RectYZ(tf2, light);

    //     scene.Add(rect);
    //     scene.AddLight(rect);
    // }

    // {
    //     auto tf2 = Transform{ Vec3(0.0f, 2.5f, -4.0f), Quat(DegToRad(40), x_axis), Vec3(4.0f, 1.0f, 1.0f) };
    //     auto light = CreateSharedRef<DiffuseLight>(Spectrum(8.0f));
    //     auto rect = RectXY(tf2, light);

    //     scene.Add(rect);
    //     scene.AddLight(rect);
    // }

    scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/sunflowers/sunflowers_4k.hdr"));
}

} // namespace spt