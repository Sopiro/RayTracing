#pragma once

#include "common.h"
#include "transform.h"

namespace bulbit
{

struct Ray
{
    Ray() = default;
    Ray(const Point3& origin, const Vec3& direction);

    Point3 At(Float t) const;

    Point3 o;
    Vec3 d;

    // todo: http://www.pbr-book.org/3ed-2018/Shapes/Managing_Rounding_Error.html
    static constexpr Float epsilon = Float(1e-4);
};

inline Ray::Ray(const Point3& origin, const Vec3& direction)
    : o{ origin }
    , d{ direction }
{
}

inline Point3 Ray::At(Float t) const
{
    return o + d * t;
}

inline Ray Mul(const Transform& tf, const Ray& ray)
{
    return Ray(Mul(tf, ray.o), tf.q.Rotate(ray.d));
}

inline Ray MulT(const Transform& tf, const Ray& ray)
{
    return Ray(MulT(tf, ray.o), tf.q.RotateInv(ray.d));
}

} // namespace bulbit