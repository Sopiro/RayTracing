#pragma once

#include "bulbit/transform.h"

namespace bulbit
{

class Mesh;
class Material;
class Scene;

bool GetAreaLightSourceCreationEnabled();
void SetAreaLightSourceCreationEnabled(bool enabled);

void CreateSphere(
    Scene& scene,
    Transform tf,
    Float radius,
    const Material* material,
    const MediumInterface& medium_interface = {},
    bool area_light = false
);
void CreateTriangles(
    Scene& scene,
    const Mesh* mesh,
    const Material* material,
    const MediumInterface& medium_interface = {},
    bool area_light = false
);

void CreateRectXY(
    Scene& scene,
    const Transform& transform,
    const Material* material,
    const MediumInterface& medium_interface = {},
    const Point2& tex_coord = Point2(1, 1),
    bool area_light = false
);
void CreateRectXZ(
    Scene& scene,
    const Transform& transform,
    const Material* material,
    const MediumInterface& medium_interface = {},
    const Point2& tex_coord = Point2(1, 1),
    bool area_light = false
);
void CreateRectYZ(
    Scene& scene,
    const Transform& transform,
    const Material* material,
    const MediumInterface& medium_interface = {},
    const Point2& tex_coord = Point2(1, 1),
    bool area_light = false
);
void CreateBox(
    Scene& scene,
    const Transform& transform,
    const Material* material,
    const MediumInterface& medium_interface = {},
    const Point2& tex_coord = Point2(1, 1),
    bool area_light = false
);

} // namespace bulbit