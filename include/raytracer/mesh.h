#pragma once

#include "common.h"
#include "texture.h"

struct Vertex
{
    Vec3 position;
    Vec3 normal;
    Vec2 texCoords;
};

class Mesh
{
public:
    std::vector<Vertex> vertices;
    std::vector<uint8> indices;
    std::vector<Texture> textures;

    // virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& rec) const override;
    // virtual bool GetAABB(AABB& outAABB) const override;
};