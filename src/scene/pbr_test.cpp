#include "raytracer/raytracer.h"

namespace spt
{

std::shared_ptr<Material> random_material()
{
    std::shared_ptr<PBRMaterial> mat = std::make_shared<PBRMaterial>();

    auto albedo = Vec3{ Prand(0.0, 1.0), Prand(0.0, 1.0), Prand(0.0, 1.0) } * 0.7;
    mat->albedo_map = SolidColor::Create(albedo);
    mat->normal_map = SolidColor::Create(0.5, 0.5, 1.0);
    mat->roughness_map = SolidColor::Create(Vec3{ Prand(0.0, 1.0) });
    mat->metallic_map = SolidColor::Create(Vec3{ Prand() > 0.8 ? 1.0 : 0.0 });
    mat->ao_map = SolidColor::Create(Vec3{ 0.0 });
    mat->emissive_map = SolidColor::Create(albedo * (Prand() < 0.04 ? Prand(0.0, 0.2) : 0.0));

    return mat;
}

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

            std::shared_ptr<Material> mat = random_material();

            scene.Add(std::make_shared<Sphere>(pos, r, mat));
        }
    }

    // Ground
    Transform tf{ Vec3{ 0.5, -r, -0.5 }, Quat{ identity }, Vec3{ 100.0 } };
    Vec3 t0 = tf * Vec3{ -0.5, 0.0, 0.5 };
    Vec3 t1 = tf * Vec3{ 0.5, 0.0, 0.5 };
    Vec3 t2 = tf * Vec3{ 0.5, 0.0, -0.5 };
    Vec3 t3 = tf * Vec3{ -0.5, 0.0, -0.5 };

    auto ground_mat = std::make_shared<PBRMaterial>();
    ground_mat->albedo_map = SolidColor::Create(Vec3{ 1.0 } * 0.9);
    ground_mat->normal_map = SolidColor::Create(0.5, 0.5, 1.0);
    ground_mat->roughness_map = SolidColor::Create(Vec3{ 0.1 });
    ground_mat->metallic_map = SolidColor::Create(Vec3{ 0.0 });
    ground_mat->ao_map = SolidColor::Create(Vec3{ 0.0 });
    ground_mat->emissive_map = SolidColor::Create(Vec3{ 0.0 });

    auto g1 = std::make_shared<Triangle>(t0, t2, t1, ground_mat);
    auto g2 = std::make_shared<Triangle>(t0, t3, t2, ground_mat);

    scene.Add(g1);
    scene.Add(g2);

    // Area light
    {
        tf = Transform{ Vec3{ -4.0, 2.5, 0.0 }, Quat{ DegToRad(50.0), Vec3{ 0.0, 0.0, 1.0 } }, Vec3{ 1.5, 1.0, 4.0 } };
        t0 = tf * Vec3{ -0.5, 0.0, 0.5 };
        t1 = tf * Vec3{ 0.5, 0.0, 0.5 };
        t2 = tf * Vec3{ 0.5, 0.0, -0.5 };
        t3 = tf * Vec3{ -0.5, 0.0, -0.5 };

        auto light = std::make_shared<DiffuseLight>(Color{ 3.0 });

        auto l1 = std::make_shared<Triangle>(t0, t2, t1, light);
        auto l2 = std::make_shared<Triangle>(t0, t3, t2, light);

        scene.Add(l1);
        scene.Add(l2);
        scene.AddLight(l1);
        scene.AddLight(l2);

        tf = Transform{ Vec3{ 4.0, 2.5, 0.0 }, Quat{ DegToRad(-50.0), Vec3{ 0.0, 0.0, 1.0 } }, Vec3{ 1.5, 1.0, 4.0 } };
        t0 = tf * Vec3{ -0.5, 0.0, 0.5 };
        t1 = tf * Vec3{ 0.5, 0.0, 0.5 };
        t2 = tf * Vec3{ 0.5, 0.0, -0.5 };
        t3 = tf * Vec3{ -0.5, 0.0, -0.5 };

        // light = std::make_shared<DiffuseLight>(Color{ 3.0 });

        // l1 = std::make_shared<Triangle>(t0, t2, t1, light);
        // l2 = std::make_shared<Triangle>(t0, t3, t2, light);

        // scene.Add(l1);
        // scene.Add(l2);
        // scene.AddLight(l1);
        // scene.AddLight(l2);
    }

    // scene.SetSkyColor(Color{ 0.5, 0.8, 0.9 } * 0.6);
}

} // namespace spt