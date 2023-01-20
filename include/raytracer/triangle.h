#pragma once

#include "common.h"
#include "hittable.h"
#include "mesh.h"
#include "ray.h"

class Triangle : public Hittable
{
public:
    Triangle() = default;
    Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2, std::shared_ptr<Material> _material, bool double_sided = true)
        : v0{ v0 }
        , v1{ v1 }
        , v2{ v2 }
        , material{ _material }
        , one_sided{ !double_sided }
    {
        e1 = v1.position - v0.position;
        e2 = v2.position - v0.position;

        face_normal = Cross(e1, e2).Normalized();
    };

    Vec3 GetNormal(Real u, Real v, Real w) const
    {
        return w * v0.normal + u * v1.normal + v * v2.normal;
    }

    Vec2 GetTexCoord(Real u, Real v, Real w) const
    {
        return w * v0.texCoords + u * v1.texCoords + v * v2.texCoords;
    }

    virtual bool Hit(const Ray& ray, Real t_min, Real t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;

public:
    Vertex v0, v1, v2;
    Vec3 e1, e2;

    Vec3 face_normal;
    std::shared_ptr<Material> material;
    bool one_sided;
};

static constexpr Vec3 epsilon_offset{ epsilon * 10.0 };

inline bool Triangle::GetAABB(AABB& outAABB) const
{
    outAABB.min = Min(Min(v0.position, v1.position), v2.position) - epsilon_offset;
    outAABB.max = Max(Max(v0.position, v1.position), v2.position) + epsilon_offset;

    return true;
}