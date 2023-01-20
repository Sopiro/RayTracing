#pragma once

#include "common.h"
#include "hittable.h"
#include "material.h"

class ConstantDensityMedium : public Hittable
{
public:
    ConstantDensityMedium(std::shared_ptr<Hittable> boundary_object, Real density, std::shared_ptr<Texture> albedo)
        : boundary{ boundary_object }
        , neg_inv_density{ Real(-1.0) / density }
        , phase_function{ std::make_shared<Isotropic>(albedo) }
    {
    }

    ConstantDensityMedium(std::shared_ptr<Hittable> boundary_object, Real density, Color color)
        : boundary{ boundary_object }
        , neg_inv_density{ Real(-1.0) / density }
        , phase_function{ std::make_shared<Isotropic>(color) }
    {
    }

    virtual bool Hit(const Ray& ray, Real t_min, Real t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;

public:
    std::shared_ptr<Hittable> boundary;
    std::shared_ptr<Material> phase_function;
    Real neg_inv_density;
};

inline bool ConstantDensityMedium::GetAABB(AABB& outAABB) const
{
    return boundary->GetAABB(outAABB);
}