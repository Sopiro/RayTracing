#pragma once

#include "onb.h"
#include "primitive.h"
#include "ray.h"

namespace spt
{

class Sphere : public Primitive
{
public:
    Sphere() = default;
    Sphere(const Vec3& center, f64 radius, const Ref<Material> material);

    virtual bool Intersect(Intersection* out_is, const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual bool IntersectAny(const Ray& ray, f64 t_min, f64 t_max) const override;
    virtual void GetAABB(AABB* out_aabb) const override;

    virtual void Sample(Intersection* sample, f64* pdf) const override;
    virtual void Sample(Intersection* sample, f64* pdf, Vec3* ref2p, const Point3& ref) const override;

    virtual f64 EvaluatePDF(const Ray& ray) const override;
    virtual f64 PDFValue(const Intersection& hit_is, const Ray& hit_ray) const override;

    virtual const Material* GetMaterial() const override;

    Vec3 center;
    f64 radius;

private:
    Ref<Material> material;
};

inline Sphere::Sphere(const Vec3& _center, f64 _radius, const Ref<Material> _material)
    : center{ _center }
    , radius{ _radius }
    , material{ _material }
{
}

inline void Sphere::GetAABB(AABB* out_aabb) const
{
    out_aabb->min = center - Vec3(radius);
    out_aabb->max = center + Vec3(radius);
}

inline f64 Sphere::EvaluatePDF(const Ray& ray) const
{
    Intersection is;
    if (Intersect(&is, ray, ray_epsilon, infinity) == false)
    {
        return 0.0;
    }

    return PDFValue(is, ray);
}
inline f64 Sphere::PDFValue(const Intersection& hit_is, const Ray& hit_ray) const
{
    f64 distance_squared = (center - hit_ray.o).Length2();
    f64 cos_theta_max = std::sqrt(1.0 - radius * radius / distance_squared);
    f64 solid_angle = two_pi * (1.0 - cos_theta_max);

    return 1.0 / solid_angle;
}

inline const Material* Sphere::GetMaterial() const
{
    return material.get();
}

} // namespace spt