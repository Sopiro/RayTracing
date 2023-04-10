#pragma once

#include "common.h"

namespace spt
{

class Ray
{
public:
    Ray() = default;
    Ray(const Point3& origin, const Vec3& dir)
        : origin{ origin }
        , dir{ dir }
    {
    }

    Point3 At(Real t) const
    {
        return origin + dir * t;
    }

public:
    Point3 origin;
    Vec3 dir;
};

} // namespace spt