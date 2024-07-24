#include "bulbit/lights.h"
#include "bulbit/material.h"
#include "bulbit/mesh.h"
#include "bulbit/microfacet.h"
#include "bulbit/scene.h"
#include "bulbit/sphere.h"
#include "bulbit/triangle.h"

#include "scene_builder.h"

#include <filesystem>

namespace bulbit
{

static bool g_create_area_light_source = true;

bool GetAreaLightSourceCreationEnabled()
{
    return g_create_area_light_source;
}

void SetAreaLightSourceCreationEnabled(bool enabled)
{
    g_create_area_light_source = enabled;
}

void CreateSphere(
    Scene& scene, Transform tf, Float radius, const Material* material, const MediumInterface& medium_interface, bool area_light
)
{
    Sphere* sphere = scene.CreateShape<Sphere>(tf, radius);
    Primitive* primitive = scene.CreatePrimitive<Primitive>(sphere, material, medium_interface);
    if (area_light || (g_create_area_light_source && material && material->GetType() == Material::Type::light_source))
    {
        scene.CreateLight<AreaLight>(primitive);
    }
}

void CreateTriangles(
    Scene& scene, const Mesh* mesh, const Material* material, const MediumInterface& medium_interface, bool area_light
)
{
    for (int32 i = 0; i < mesh->GetTriangleCount(); ++i)
    {
        Triangle* triangle = scene.CreateShape<Triangle>(mesh, i);
        Primitive* primitive = scene.CreatePrimitive<Primitive>(triangle, material, medium_interface);
        if (area_light || (g_create_area_light_source && material && material->GetType() == Material::Type::light_source))
        {
            scene.CreateLight<AreaLight>(primitive);
        }
    }
}

void CreateRectXY(
    Scene& scene,
    const Transform& tf,
    const Material* mat,
    const MediumInterface& medium_interface,
    const Point2& tc,
    bool area_light
)
{
    Vec3 p0 = { -0.5, -0.5, 0.0 };
    Vec3 p1 = { 0.5, -0.5, 0.0 };
    Vec3 p2 = { 0.5, 0.5, 0.0 };
    Vec3 p3 = { -0.5, 0.5, 0.0 };

    Vertex v0{ p0, z_axis, x_axis, Point2(0.0, 0.0) };
    Vertex v1{ p1, z_axis, x_axis, Point2(tc.x, 0.0) };
    Vertex v2{ p2, z_axis, x_axis, Point2(tc.x, tc.y) };
    Vertex v3{ p3, z_axis, x_axis, Point2(0.0, tc.y) };

    auto vertices = std::vector<Vertex>{ v0, v1, v2, v3 };
    auto indices = std::vector<int32>{ 0, 1, 2, 0, 2, 3 };

    Mesh* m = scene.CreateMesh(vertices, indices, tf);
    CreateTriangles(scene, m, mat, medium_interface, area_light);
};

void CreateRectXZ(
    Scene& scene,
    const Transform& tf,
    const Material* mat,
    const MediumInterface& medium_interface,
    const Point2& tc,
    bool area_light
)
{
    Vec3 p0 = { -0.5, 0.0, 0.5 };
    Vec3 p1 = { 0.5, 0.0, 0.5 };
    Vec3 p2 = { 0.5, 0.0, -0.5 };
    Vec3 p3 = { -0.5, 0.0, -0.5 };

    Vertex v0{ p0, y_axis, x_axis, Point2(0.0, 0.0) };
    Vertex v1{ p1, y_axis, x_axis, Point2(tc.x, 0.0) };
    Vertex v2{ p2, y_axis, x_axis, Point2(tc.x, tc.y) };
    Vertex v3{ p3, y_axis, x_axis, Point2(0.0, tc.y) };

    auto vertices = std::vector<Vertex>{ v0, v1, v2, v3 };
    auto indices = std::vector<int32>{ 0, 1, 2, 0, 2, 3 };

    Mesh* m = scene.CreateMesh(vertices, indices, tf);
    CreateTriangles(scene, m, mat, medium_interface, area_light);
}

void CreateRectYZ(
    Scene& scene,
    const Transform& tf,
    const Material* mat,
    const MediumInterface& medium_interface,
    const Point2& tc,
    bool area_light
)
{
    Vec3 p0 = { 0.0, -0.5, 0.5 };
    Vec3 p1 = { 0.0, -0.5, -0.5 };
    Vec3 p2 = { 0.0, 0.5, -0.5 };
    Vec3 p3 = { 0.0, 0.5, 0.5 };

    Vertex v0{ p0, x_axis, -z_axis, Point2(0.0, 0.0) };
    Vertex v1{ p1, x_axis, -z_axis, Point2(tc.x, 0.0) };
    Vertex v2{ p2, x_axis, -z_axis, Point2(tc.x, tc.y) };
    Vertex v3{ p3, x_axis, -z_axis, Point2(0.0, tc.y) };

    auto vertices = std::vector<Vertex>{ v0, v1, v2, v3 };
    auto indices = std::vector<int32>{ 0, 1, 2, 0, 2, 3 };

    Mesh* m = scene.CreateMesh(vertices, indices, tf);
    CreateTriangles(scene, m, mat, medium_interface, area_light);
}

void CreateBox(
    Scene& scene,
    const Transform& tf,
    const Material* mat,
    const MediumInterface& medium_interface,
    const Point2& tc,
    bool area_light
)
{
    /*
          7--------6
         /|       /|
        3--------2 |
        | 4------|-5
        |/       |/
        0--------1
    */
    Vec3 p0 = { -0.5, -0.5, 0.5 };
    Vec3 p1 = { 0.5, -0.5, 0.5 };
    Vec3 p2 = { 0.5, 0.5, 0.5 };
    Vec3 p3 = { -0.5, 0.5, 0.5 };

    Vec3 p4 = { -0.5, -0.5, -0.5 };
    Vec3 p5 = { 0.5, -0.5, -0.5 };
    Vec3 p6 = { 0.5, 0.5, -0.5 };
    Vec3 p7 = { -0.5, 0.5, -0.5 };

    Vertex v00 = { p0, z_axis, x_axis, Point2(0.0, 0.0) };
    Vertex v01 = { p1, z_axis, x_axis, Point2(tc.x, 0.0) };
    Vertex v02 = { p2, z_axis, x_axis, Point2(tc.x, tc.y) };
    Vertex v03 = { p3, z_axis, x_axis, Point2(0.0, tc.y) };

    Vertex v04 = { p1, x_axis, -z_axis, Point2(0.0, 0.0) };
    Vertex v05 = { p5, x_axis, -z_axis, Point2(tc.x, 0.0) };
    Vertex v06 = { p6, x_axis, -z_axis, Point2(tc.x, tc.y) };
    Vertex v07 = { p2, x_axis, -z_axis, Point2(0.0, tc.y) };

    Vertex v08 = { p5, -z_axis, -x_axis, Point2(0.0, 0.0) };
    Vertex v09 = { p4, -z_axis, -x_axis, Point2(tc.x, 0.0) };
    Vertex v10 = { p7, -z_axis, -x_axis, Point2(tc.x, tc.y) };
    Vertex v11 = { p6, -z_axis, -x_axis, Point2(0.0, tc.y) };

    Vertex v12 = { p4, -x_axis, z_axis, Point2(0.0, 0.0) };
    Vertex v13 = { p0, -x_axis, z_axis, Point2(tc.x, 0.0) };
    Vertex v14 = { p3, -x_axis, z_axis, Point2(tc.x, tc.y) };
    Vertex v15 = { p7, -x_axis, z_axis, Point2(0.0, tc.y) };

    Vertex v16 = { p3, y_axis, x_axis, Point2(0.0, 0.0) };
    Vertex v17 = { p2, y_axis, x_axis, Point2(tc.x, 0.0) };
    Vertex v18 = { p6, y_axis, x_axis, Point2(tc.x, tc.y) };
    Vertex v19 = { p7, y_axis, x_axis, Point2(0.0, tc.y) };

    Vertex v20 = { p1, -y_axis, -x_axis, Point2(0.0, 0.0) };
    Vertex v21 = { p0, -y_axis, -x_axis, Point2(tc.x, 0.0) };
    Vertex v22 = { p4, -y_axis, -x_axis, Point2(tc.x, tc.y) };
    Vertex v23 = { p5, -y_axis, -x_axis, Point2(0.0, tc.y) };

    auto vertices = std::vector<Vertex>{ v00, v01, v02, v03, v04, v05, v06, v07, v08, v09, v10, v11,
                                         v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23 };

    // clang-format off
    auto indices = std::vector<int32>{
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };
    // clang-format on

    Mesh* m = scene.CreateMesh(vertices, indices, tf);
    CreateTriangles(scene, m, mat, medium_interface, area_light);
}

} // namespace bulbit
