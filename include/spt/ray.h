#pragma once

#include "common.h"

namespace spt
{

class Ray
{
public:
    Ray() = default;
    Ray(const Point3& origin, const Vec3& dir);

    Point3 At(f64 t) const;

public:
    Point3 origin;
    Vec3 dir;
};

inline Ray::Ray(const Point3& origin, const Vec3& dir)
    : origin{ origin }
    , dir{ dir }
{
}

inline Point3 Ray::At(f64 t) const
{
    return origin + dir * t;
}

} // namespace spt