#pragma once

#include "ray.h"
#include "shape.h"

namespace bulbit
{

class Sphere : public Shape
{
public:
    Sphere(const Vec3& center, Float radius);
    Sphere(const Transform& transform, Float radius);

    virtual AABB GetAABB() const override;
    virtual bool Intersect(Intersection* out_isect, const Ray& ray, Float t_min, Float t_max) const override;
    virtual bool IntersectAny(const Ray& ray, Float t_min, Float t_max) const override;

    virtual ShapeSample Sample(const Point2& u) const override;
    virtual ShapeSample Sample(const Point3& ref, const Point2& u) const override;

    virtual Float EvaluatePDF(const Ray& ray) const override;
    virtual Float PDF(const Intersection& hit_is, const Ray& hit_ray) const override;

    Transform transform;
    Float radius;

private:
    static Point2 ComputeTexCoord(const Vec3& v);
};

inline Sphere::Sphere(const Vec3& center, Float radius)
    : transform{ center, identity }
    , radius{ radius }
{
}

inline Sphere::Sphere(const Transform& transform, Float radius)
    : transform{ transform }
    , radius{ radius }
{
}

inline AABB Sphere::GetAABB() const
{
    return AABB(transform.p - Vec3(radius), transform.p + Vec3(radius));
}

inline Float Sphere::EvaluatePDF(const Ray& ray) const
{
    Intersection is;
    if (Intersect(&is, ray, Ray::epsilon, infinity) == false)
    {
        return 0.0f;
    }

    return PDF(is, ray);
}

inline Float Sphere::PDF(const Intersection& hit_is, const Ray& hit_ray) const
{
    Float distance_squared = Length2(transform.p - hit_ray.o);
    Float cos_theta_max = std::sqrt(1 - radius * radius / distance_squared);
    Float solid_angle = two_pi * (1 - cos_theta_max);

    return 1 / solid_angle;
}

inline Point2 Sphere::ComputeTexCoord(const Vec3& v)
{
    Float theta = std::acos(Clamp(v.y, -1, 1));
    Float r = std::atan2(v.z, v.x);
    Float phi = r < 0 ? r + two_pi : r;

    return Point2(phi * inv_two_pi, 1 - theta * inv_pi);
}

} // namespace bulbit