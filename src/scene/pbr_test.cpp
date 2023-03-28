#include "raytracer/raytracer.h"

namespace spt
{

void PBRTest(Scene& scene)
{
    // Srand(1234);

    double r = 0.3;
    double cx = 10.0;
    double cz = 7.0;
    double xgap = 0.16;
    double zgap = 0.14;
    double xstep = 2.0 * r + xgap;
    double zstep = 2.0 * r + zgap;

    for (int32 z = 0; z < cz; ++z)
    {
        for (int32 x = 0; x < cx; ++x)
        {
            Vec3 pos;

            pos.y = 0.0;
            pos.x = x * xstep - ((cx - 1) * xstep / 2.0);
            pos.z = z * zstep - ((cz - 1) * zstep / 2.0);

            std::shared_ptr<Material> mat = RandomMaterial();

            scene.Add(std::make_shared<Sphere>(pos, r, mat));
        }
    }

    // Ground

    auto ground_mat = std::make_shared<PBRMaterial>();
    ground_mat->albedo_map = SolidColor::Create(Vec3{ 1.0 } * 0.9);
    ground_mat->normal_map = SolidColor::Create(0.5, 0.5, 1.0);
    ground_mat->roughness_map = SolidColor::Create(Vec3{ 0.1 });
    ground_mat->metallic_map = SolidColor::Create(Vec3{ 0.0 });
    ground_mat->ao_map = SolidColor::Create(Vec3{ 0.0 });
    ground_mat->emissive_map = SolidColor::Create(Vec3{ 0.0 });

    auto tf1 = Transform{ Vec3{ 0.5, -r, -0.5 }, Quat{ identity }, Vec3{ 100.0 } };
    auto ground = RectXZ(tf1, ground_mat);

    scene.Add(ground);

    // Area light
    auto tf2 = Transform{ Vec3{ -4.0, 2.5, 0.0 }, Quat{ DegToRad(50.0), Vec3{ 0.0, 0.0, 1.0 } }, Vec3{ 1.5, 1.0, 4.0 } };
    auto light = std::make_shared<DiffuseLight>(Color{ 3.0 });

    auto rect = RectXZ(tf2, light);

    scene.Add(rect);
    scene.AddLight(rect);

    // scene.SetSkyColor(Color{ 0.5, 0.8, 0.9 } * 0.6);
}

} // namespace spt