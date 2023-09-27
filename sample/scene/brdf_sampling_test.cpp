#include "spt/spt.h"

namespace spt
{

void BRDFSamplingTest(Scene& scene)
{
    // Materials
    auto red = CreateSharedRef<Lambertian>(Color(Float(.65), Float(.05), Float(.05)));
    auto green = CreateSharedRef<Lambertian>(Color(Float(.12), Float(.45), Float(.15)));
    auto blue = CreateSharedRef<Lambertian>(Color(Float(.22), Float(.23), Float(.75)));
    auto white = CreateSharedRef<Lambertian>(Color(Float(.73), Float(.73), Float(.73)));
    auto wakgood_texture = ImageTexture::Create("res/wakdu.jpg");
    auto wakgood_mat = CreateSharedRef<Lambertian>(wakgood_texture);
    auto light = CreateSharedRef<DiffuseLight>(Color(15.0));

    // Cornell box
    {
        // front
        auto tf = Transform{ Vec3(0.5, 0.5, -1.0), Quat(identity), Vec3(1.0) };
        scene.Add(CreateRectXY(tf, wakgood_mat));

        // left
        tf = Transform{ Vec3(0.0, 0.5, -0.5), Quat(identity), Vec3(1.0) };
        scene.Add(CreateRectYZ(tf, red));

        // right
        tf = Transform{ Vec3(1.0, 0.5, -0.5), Quat(pi, y_axis), Vec3(1.0) };
        scene.Add(CreateRectYZ(tf, green));

        // bottom
        tf = Transform{ Vec3(0.5, 0, -0.5), Quat(identity), Vec3(1.0) };
        scene.Add(CreateRectXZ(tf, white));

        // top
        tf = Transform{ Vec3(0.5, 1.0, -0.5), Quat(pi, x_axis), Vec3(1.0) };
        scene.Add(CreateRectXZ(tf, white));
    }

    // Lights
    {
        auto tf = Transform{ 0.5, 0.999, -0.5, Quat(pi, x_axis), Vec3(0.25) };
        auto l = CreateRectXZ(tf, light);

        scene.AddLight(l);
    }

    // Center sphere
    {
        auto mat = RandomMicrofacetMaterial();
        mat->basecolor = SolidColor::Create(Vec3(1.0));
        mat->metallic = SolidColor::Create(Vec3(1.0));
        mat->roughness = SolidColor::Create(Vec3(0.2));

        Float r = 0.25;
        auto sphere = CreateSharedRef<Sphere>(Vec3(0.5, r, -0.5), r, mat);

        scene.Add(sphere);
        // scene.AddLight(sphere);
    }

    // scene.Rebuild();
    // std::cout << scene.GetLights().GetCount() << std::endl;
}

} // namespace spt