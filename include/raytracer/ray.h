#pragma once

#include "common.h"

namespace spt
{

class Ray
{
public:
    Ray() = default;
    Ray(const Point& origin, const Vec3& dir)
        : origin{ origin }
        , dir{ dir }
    {
    }

    Point At(Real t) const
    {
        return origin + dir * t;
    }

public:
    Point origin;
    Vec3 dir;
};

} // namespace spt