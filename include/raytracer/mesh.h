#pragma once

#include "bvh.h"
#include "common.h"
#include "material.h"

namespace spt
{

class Triangle;

struct Vertex
{
    Vec3 position;
    Vec3 normal;
    Vec2 texCoords;
};

class Mesh : public Hittable
{
public:
    Mesh(std::vector<Vertex> vertices,
         std::vector<uint32> indices,
         std::vector<std::shared_ptr<Texture>> textures,
         const Mat4& transform);

    virtual bool Hit(const Ray& ray, Real t_min, Real t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;

private:
    Mat4 transform;

    BVH bvh;

    std::vector<Triangle> triangles;
    std::vector<Vertex> vertices;
    std::vector<uint32> indices;
    std::vector<std::shared_ptr<Texture>> textures;
};

} // namespace spt