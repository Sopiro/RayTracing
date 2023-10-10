#pragma once

#include "intersectable.h"
#include "isotropic.h"

namespace spt
{

class ConstantDensityMedium : public Intersectable
{
public:
    ConstantDensityMedium(const Ref<Intersectable> boundary_object, Float density, const Ref<Texture> albedo);
    ConstantDensityMedium(const Ref<Intersectable> boundary_object, Float density, Spectrum color);

    virtual bool Intersect(Intersection* out_is, const Ray& ray, Float t_min, Float t_max) const override;
    virtual bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const override;
    virtual void GetAABB(AABB* out_aabb) const override;

public:
    Ref<Intersectable> boundary;
    Ref<Material> phase_function;
    Float neg_inv_density;
};

inline bool ConstantDensityMedium::IntersectAny(const Ray& ray, Float t_min, Float t_max) const
{
    return boundary->IntersectAny(ray, t_min, t_max);
}

inline void ConstantDensityMedium::GetAABB(AABB* out_aabb) const
{
    boundary->GetAABB(out_aabb);
}

} // namespace spt