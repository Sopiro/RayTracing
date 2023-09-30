#pragma once

#include "primitive.h"
#include "ray.h"

namespace spt
{

class Sphere : public Primitive
{
public:
    Sphere() = default;
    Sphere(const Vec3& center, Float radius, const Ref<Material> material);

    virtual void GetAABB(AABB* out_aabb) const override;
    virtual bool Intersect(Intersection* out_is, const Ray& ray, Float t_min, Float t_max) const override;
    virtual bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const override;

    virtual void Sample(Intersection* sample, Float* pdf) const override;
    virtual void Sample(Intersection* sample, Float* pdf, Vec3* ref2p, const Point3& ref) const override;

    virtual Float EvaluatePDF(const Ray& ray) const override;
    virtual Float PDFValue(const Intersection& hit_is, const Ray& hit_ray) const override;

    virtual const Material* GetMaterial() const override;

    Vec3 center;
    Float radius;

private:
    Ref<Material> material;
};

inline Sphere::Sphere(const Vec3& _center, Float _radius, const Ref<Material> _material)
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

inline Float Sphere::EvaluatePDF(const Ray& ray) const
{
    Intersection is;
    if (Intersect(&is, ray, Ray::epsilon, infinity) == false)
    {
        return Float(0.0);
    }

    return PDFValue(is, ray);
}
inline Float Sphere::PDFValue(const Intersection& hit_is, const Ray& hit_ray) const
{
    Float distance_squared = (center - hit_ray.o).Length2();
    Float cos_theta_max = std::sqrt(1 - radius * radius / distance_squared);
    Float solid_angle = two_pi * (1 - cos_theta_max);

    return 1 / solid_angle;
}

inline const Material* Sphere::GetMaterial() const
{
    return material.get();
}

} // namespace spt