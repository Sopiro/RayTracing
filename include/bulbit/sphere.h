#pragma once

#include "primitive.h"
#include "ray.h"

namespace bulbit
{

class Sphere : public Primitive
{
public:
    Sphere() = default;
    Sphere(const Vec3& center, Float radius, MaterialIndex material);

    virtual AABB GetAABB() const override;
    virtual bool Intersect(Intersection* out_is, const Ray& ray, Float t_min, Float t_max) const override;
    virtual bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const override;

    virtual void Sample(Intersection* sample, Float* pdf, const Point2& u) const override;
    virtual void Sample(Intersection* sample, Float* pdf, Vec3* ref2p, const Point3& ref, const Point2& u) const override;

    virtual Float EvaluatePDF(const Ray& ray) const override;
    virtual Float PDFValue(const Intersection& hit_is, const Ray& hit_ray) const override;

    virtual MaterialIndex GetMaterialIndex() const override;

    Vec3 center;
    Float radius;

private:
    static inline Point2 ComputeTexCoord(const Vec3& v);

    MaterialIndex material;
};

inline Sphere::Sphere(const Vec3& _center, Float _radius, MaterialIndex _material)
    : center{ _center }
    , radius{ _radius }
    , material{ _material }
{
}

inline AABB Sphere::GetAABB() const
{
    return AABB(center - Vec3(radius), center + Vec3(radius));
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

inline MaterialIndex Sphere::GetMaterialIndex() const
{
    return material;
}

inline Point2 Sphere::ComputeTexCoord(const Vec3& v)
{
    Float theta = std::acos(v.y);
    Float r = std::atan2(v.z, v.x);
    Float phi = r < 0 ? r + two_pi : r;

    return Point2(phi * inv_two_pi, 1 - theta * inv_pi);
}

} // namespace bulbit