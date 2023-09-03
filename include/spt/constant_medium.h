#pragma once

#include "common.h"
#include "intersectable.h"
#include "isotropic.h"
#include "material.h"

namespace spt
{

class ConstantDensityMedium : public Intersectable
{
public:
    ConstantDensityMedium(const Ref<Intersectable>& boundary_object, f64 density, const Ref<Texture>& albedo);
    ConstantDensityMedium(const Ref<Intersectable>& boundary_object, f64 density, Color color);

    virtual bool Intersect(const Ray& ray, f64 t_min, f64 t_max, Intersection& is) const override;
    virtual bool GetAABB(AABB& out_aabb) const override;
    virtual const Material* GetMaterial() const override;

public:
    Ref<Intersectable> boundary;
    Ref<Material> phase_function;
    f64 neg_inv_density;
};

inline ConstantDensityMedium::ConstantDensityMedium(const Ref<Intersectable>& boundary_object,
                                                    f64 density,
                                                    const Ref<Texture>& albedo)
    : boundary{ std::move(boundary_object) }
    , neg_inv_density{ -1.0 / density }
    , phase_function{ CreateSharedRef<Isotropic>(albedo) }
{
}

inline ConstantDensityMedium::ConstantDensityMedium(const Ref<Intersectable>& boundary_object, f64 density, Color color)
    : boundary{ boundary_object }
    , neg_inv_density{ -1.0 / density }
    , phase_function{ CreateSharedRef<Isotropic>(color) }
{
}

inline bool ConstantDensityMedium::GetAABB(AABB& out_aabb) const
{
    return boundary->GetAABB(out_aabb);
}

inline const Material* ConstantDensityMedium::GetMaterial() const
{
    return phase_function.get();
}

} // namespace spt