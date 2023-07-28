#pragma once

#include "common.h"
#include "hittable.h"
#include "isotropic.h"
#include "material.h"

namespace spt
{

class ConstantDensityMedium : public Hittable
{
public:
    ConstantDensityMedium(const Ref<Hittable>& boundary_object, f64 density, const Ref<Texture>& albedo);
    ConstantDensityMedium(const Ref<Hittable>& boundary_object, f64 density, Color color);

    virtual bool Hit(const Ray& ray, f64 t_min, f64 t_max, HitRecord& rec) const override;
    virtual bool GetAABB(AABB& outAABB) const override;

public:
    Ref<Hittable> boundary;
    Ref<Material> phase_function;
    f64 neg_inv_density;
};

inline ConstantDensityMedium::ConstantDensityMedium(const Ref<Hittable>& boundary_object,
                                                    f64 density,
                                                    const Ref<Texture>& albedo)
    : boundary{ std::move(boundary_object) }
    , neg_inv_density{ -1.0 / density }
    , phase_function{ CreateSharedRef<Isotropic>(albedo) }
{
}

inline ConstantDensityMedium::ConstantDensityMedium(const Ref<Hittable>& boundary_object, f64 density, Color color)
    : boundary{ boundary_object }
    , neg_inv_density{ -1.0 / density }
    , phase_function{ CreateSharedRef<Isotropic>(color) }
{
}

inline bool ConstantDensityMedium::GetAABB(AABB& outAABB) const
{
    return boundary->GetAABB(outAABB);
}

} // namespace spt