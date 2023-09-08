#pragma once

#include "common.h"

namespace spt
{

struct Ray
{
    Ray() = default;
    Ray(const Point3& origin, const Vec3& direction);

    Point3 At(f64 t) const;

    Point3 origin;
    Vec3 dir;
};

inline Ray::Ray(const Point3& _origin, const Vec3& _direction)
    : origin{ _origin }
    , dir{ _direction }
{
}

inline Point3 Ray::At(f64 t) const
{
    return origin + dir * t;
}

} // namespace spt