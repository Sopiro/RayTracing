#pragma once

#include "common.h"

namespace spt
{

constexpr f64 ray_offset = 1e-4;

struct Ray
{
    Ray() = default;
    Ray(const Point3& origin, const Vec3& direction);

    Point3 At(f64 t) const;

    Point3 o;
    Vec3 d;
};

inline Ray::Ray(const Point3& origin, const Vec3& direction)
    : o{ origin }
    , d{ direction }
{
}

inline Point3 Ray::At(f64 t) const
{
    return o + d * t;
}

} // namespace spt