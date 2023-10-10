#include "spt/spt.h"
#include "spt/util.h"

namespace spt
{

void EnvironmentMap(Scene& scene)
{
    Float r = 1.0;
    Float cx = 4.0;
    Float cz = 1.0;
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

            // auto mat = RandomMaterial();

            Ref<Material> mat;
            if (x >= 2)
            {
                mat = CreateSharedRef<Dielectric>(1.5);
            }
            else
            {
                mat = CreateSharedRef<Metal>(Vec3(0.6), 0.0);
            }

            if (x % 2)
            {
                scene.Add(CreateSharedRef<Sphere>(pos, r, mat));
            }
            else
            {
                auto tf = Transform{ pos, Quat(identity), Vec3(r * 1.414) };
                scene.Add(CreateBox(tf, mat));
            }
        }
    }

    // Ground
    // {
    //     auto mat = CreateSharedRef<PBRMaterial>();
    //     mat->albedo_map = ConstantColor::Create(Spectrum(1.0) * 0.9);
    //     mat->normal_map = ConstantColor::Create(0.5, 0.5, 1.0);
    //     mat->roughness_map = ConstantColor::Create(Spectrum(0.1));
    //     mat->metallic_map = ConstantColor::Create(Spectrum(0.0));
    //     mat->ao_map = ConstantColor::Create(Spectrum(1.0));
    //     mat->emissive_map = ConstantColor::Create(Spectrum(0.0));

    //     auto tf1 = Transform{ Vec3(0.5, -r, -0.5), Quat(identity), Vec3(100.0) };
    //     auto ground = RectXZ(tf1, mat);

    //     scene.Add(ground);
    // }

    // // Light
    // {
    //     auto tf2 = Transform{ Vec3(-4.0, 2.5, 0.0), Quat(DegToRad(-40.0), z_axis), Vec3(1.0, 1.0, 4.0) };
    //     auto light = CreateSharedRef<DiffuseLight>(Spectrum(8.0));
    //     auto rect = RectYZ(tf2, light);

    //     scene.Add(rect);
    //     scene.AddLight(rect);
    // }

    // {
    //     auto tf2 = Transform{ Vec3(4.0, 2.5, 0.0), Quat(DegToRad(180 + 50), z_axis), Vec3(1.0, 1.0, 4.0) };
    //     auto light = CreateSharedRef<DiffuseLight>(Spectrum(8.0));
    //     auto rect = RectYZ(tf2, light);

    //     scene.Add(rect);
    //     scene.AddLight(rect);
    // }

    // {
    //     auto tf2 = Transform{ Vec3(0.0, 2.5, -4.0), Quat(DegToRad(40), x_axis), Vec3(4.0, 1.0, 1.0) };
    //     auto light = CreateSharedRef<DiffuseLight>(Spectrum(8.0));
    //     auto rect = RectXY(tf2, light);

    //     scene.Add(rect);
    //     scene.AddLight(rect);
    // }

    scene.AddLight(CreateSharedRef<InfiniteAreaLight>("res/sunflowers/sunflowers_4k.hdr"));
}

} // namespace spt