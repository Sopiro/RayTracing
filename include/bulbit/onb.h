#pragma once

#include "common.h"

namespace bulbit
{

// Orthonormal basis
// Only concern about polar angle
struct ONB
{
    ONB() = default;
    ONB(const Vec3& n);

    // Convert from local coordinates to world coordinates
    Vec3 FromLocal(const Vec3& v) const;

    // Convert from world coordinates to local coordinates
    Vec3 ToLocal(const Vec3& v) const;

    Vec3& operator[](int32 i);
    Vec3 operator[](int32 i) const;

    Vec3 u, v, w;
};

inline ONB::ONB(const Vec3& n)
    : w{ n }
{
    CoordinateSystem(n, &u, &v);
}

inline Vec3& ONB::operator[](int32 i)
{
    return (&u)[i];
}

inline Vec3 ONB::operator[](int32 i) const
{
    return (&u)[i];
}

inline Vec3 ONB::FromLocal(const Vec3& d) const
{
    return d.x * u + d.y * v + d.z * w;
}

inline Vec3 ONB::ToLocal(const Vec3& d) const
{
    return Vec3(Dot(d, u), Dot(d, v), Dot(d, w));
}

} // namespace bulbit
