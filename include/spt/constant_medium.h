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

    virtual bool Intersect(Intersection* out_is, const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual bool GetAABB(AABB* out_aabb) const override;
    virtual f64 EvaluatePDF(const Ray& ray) const override;
    virtual f64 PDFValue(const Intersection& hit_is, const Ray& hit_ray) const override;
    virtual Vec3 GetRandomDirection(const Point3& origin) const override;
    virtual i32 GetSize() const override;
    virtual void Rebuild() override;
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

inline bool ConstantDensityMedium::GetAABB(AABB* out_aabb) const
{
    return boundary->GetAABB(out_aabb);
}

inline f64 ConstantDensityMedium::EvaluatePDF(const Ray& ray) const
{
    return boundary->EvaluatePDF(ray);
}

inline f64 ConstantDensityMedium::PDFValue(const Intersection& hit_is, const Ray& hit_ray) const
{
    return boundary->PDFValue(hit_is, hit_ray);
}

inline Vec3 ConstantDensityMedium::GetRandomDirection(const Point3& origin) const
{
    return boundary->GetRandomDirection(origin);
}

inline i32 ConstantDensityMedium::GetSize() const
{
    return boundary->GetSize();
}

inline void ConstantDensityMedium::Rebuild()
{
    boundary->Rebuild();
}

inline const Material* ConstantDensityMedium::GetMaterial() const
{
    return phase_function.get();
}

} // namespace spt