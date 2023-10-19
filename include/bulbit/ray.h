#pragma once

#include "common.h"

namespace spt
{

struct Ray
{
    Ray() = default;
    Ray(const Point3& origin, const Vec3& direction);

    Point3 At(Float t) const;

    Point3 o;
    Vec3 d;

    // todo: http://www.pbr-book.org/3ed-2018/Shapes/Managing_Rounding_Error.html
    inline static constexpr Float epsilon = Float(1e-4);
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

} // namespace spt