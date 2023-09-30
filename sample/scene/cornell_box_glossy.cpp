#include "spt/spt.h"

namespace spt
{

void CornellBoxGlossy(Scene& scene)
{
    // Materials
    auto red = CreateSharedRef<Lambertian>(Color(.65, .05, .05));
    auto green = CreateSharedRef<Lambertian>(Color(.12, .45, .15));
    auto blue = CreateSharedRef<Lambertian>(Color(.22, .23, .75));
    auto white = CreateSharedRef<Lambertian>(Color(.73, .73, .73));
    auto wakgood_texture = ImageTexture::Create("res/wakdu.jpg");
    auto wakgood_mat = CreateSharedRef<Lambertian>(wakgood_texture);
    auto light = CreateSharedRef<DiffuseLight>(Color(15.0));
    auto mirror = CreateSharedRef<Metal>(Color(.73, .73, .73), 0.0);
    auto glass = CreateSharedRef<Dielectric>(1.5);

    // Cornell box
    {
        // front

        auto mat = RandomMicrofacetMaterial();
        mat->basecolor = wakgood_texture;
        mat->roughness = ConstantColor::Create(0.01);
        mat->metallic = ConstantColor::Create(0.0);
        mat->emissive = ConstantColor::Create(0.0);

        auto tf = Transform{ Vec3(0.5, 0.5, -1.0), Quat(identity), Vec3(1.0) };
        scene.Add(CreateRectXY(tf, mat));

        // left

        mat = RandomMicrofacetMaterial();
        mat->basecolor = ConstantColor::Create(.65, .05, .05);
        mat->roughness = ConstantColor::Create(0.01);
        mat->metallic = ConstantColor::Create(0.0);
        mat->emissive = ConstantColor::Create(0.0);
        tf = Transform{ Vec3(0.0, 0.5, -0.5), Quat(identity), Vec3(1.0) };
        scene.Add(CreateRectYZ(tf, mat));

        // right

        mat = RandomMicrofacetMaterial();
        mat->basecolor = ConstantColor::Create(.12, .45, .15);
        mat->roughness = ConstantColor::Create(0.01);
        mat->metallic = ConstantColor::Create(0.0);
        mat->emissive = ConstantColor::Create(0.0);
        tf = Transform{ Vec3(1.0, 0.5, -0.5), Quat(pi, y_axis), Vec3(1.0) };
        scene.Add(CreateRectYZ(tf, mat));

        // bottom
        mat = RandomMicrofacetMaterial();
        mat->basecolor = ConstantColor::Create(.73);
        mat->roughness = ConstantColor::Create(0.01);
        mat->metallic = ConstantColor::Create(0.0);
        mat->emissive = ConstantColor::Create(0.0);
        tf = Transform{ Vec3(0.5, 0, -0.5), Quat(identity), Vec3(1.0) };
        scene.Add(CreateRectXZ(tf, mat));

        // top
        mat = RandomMicrofacetMaterial();
        mat->basecolor = ConstantColor::Create(.73);
        mat->roughness = ConstantColor::Create(0.01);
        mat->metallic = ConstantColor::Create(0.0);
        mat->emissive = ConstantColor::Create(0.0);
        tf = Transform{ Vec3(0.5, 1.0, -0.5), Quat(pi, x_axis), Vec3(1.0) };
        scene.Add(CreateRectXZ(tf, mat));
    }

    // Left block
    {
        Float hx = 0.13;
        Float hy = 0.26;
        Float hz = 0.13;

        auto mat = RandomMicrofacetMaterial();
        mat->basecolor = ConstantColor::Create(1.0);
        mat->roughness = ConstantColor::Create(0.1);
        mat->metallic = ConstantColor::Create(1.0);
        mat->emissive = ConstantColor::Create(0.0);

        auto tf = Transform{ 0.3, hy, -0.6, Quat(DegToRad(25.0), y_axis), Vec3(hx * 2.0, hy * 2.0, hz * 2.0) };
        auto box = CreateBox(tf, mat);

        scene.Add(box);
    }

    // Right bunny
    {
        auto mat = RandomMicrofacetMaterial();
        mat->basecolor = ConstantColor::Create(0.7);
        mat->roughness = ConstantColor::Create(0.05);
        mat->metallic = ConstantColor::Create(1.0);
        mat->emissive = ConstantColor::Create(0.0);

        // Bunny
        Transform tf{ Point3(0.7, 0.0, -0.3), Quat(identity), Vec3(0.3) };

        Ref<Model> model = CreateSharedRef<Model>("res/stanford/bunny.obj", tf);
        model->GetMeshes()[0]->SetMaterial(mat);
        scene.Add(model);
    }

    // Lights
    {
        auto tf = Transform{ 0.5, 0.999, -0.5, Quat(pi, x_axis), Vec3(0.25) };
        auto l = CreateRectXZ(tf, light);

        scene.AddLight(l);
    }

    // scene.Rebuild();
    // std::cout << "Lights: " << scene.GetLights().GetCount() << std::endl;
}

} // namespace spt