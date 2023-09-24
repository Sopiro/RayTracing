#pragma once

#include "intersectable.h"
#include "isotropic.h"

namespace spt
{

class ConstantDensityMedium : public Intersectable
{
public:
    ConstantDensityMedium(const Ref<Intersectable> boundary_object, f64 density, const Ref<Texture> albedo);
    ConstantDensityMedium(const Ref<Intersectable> boundary_object, f64 density, Color color);

    virtual bool Intersect(Intersection* out_is, const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual bool IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual void GetAABB(AABB* out_aabb) const override;

public:
    Ref<Intersectable> boundary;
    Ref<Material> phase_function;
    f64 neg_inv_density;
};

inline ConstantDensityMedium::ConstantDensityMedium(const Ref<Intersectable> boundary_object,
                                                    f64 density,
                                                    const Ref<Texture> albedo)
    : boundary{ std::move(boundary_object) }
    , neg_inv_density{ -1.0 / density }
    , phase_function{ CreateSharedRef<Isotropic>(albedo) }
{
}

inline ConstantDensityMedium::ConstantDensityMedium(const Ref<Intersectable> boundary_object, f64 density, Color color)
    : boundary{ boundary_object }
    , neg_inv_density{ -1.0 / density }
    , phase_function{ CreateSharedRef<Isotropic>(color) }
{
}

inline bool ConstantDensityMedium::IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const
{
    return boundary->IntersectAny(ray, t_min, t_max);
}

inline void ConstantDensityMedium::GetAABB(AABB* out_aabb) const
{
    boundary->GetAABB(out_aabb);
}

} // namespace spt