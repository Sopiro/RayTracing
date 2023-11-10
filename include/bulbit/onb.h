#pragma once

#include "common.h"

namespace bulbit
{

// Represents orthonormal coordinate frame
struct Frame
{
    static Frame FromXZ(const Vec3& x, const Vec3& z);
    static Frame FromXY(const Vec3& x, const Vec3& y);
    static Frame FromX(const Vec3& x);
    static Frame FromY(const Vec3& y);
    static Frame FromZ(const Vec3& z);

    Frame() = default;
    Frame(const Vec3& n);
    Frame(const Vec3& x, const Vec3& y, const Vec3& z);

    // Convert from local coordinates to world coordinates
    Vec3 FromLocal(const Vec3& v) const;

    // Convert from world coordinates to local coordinates
    Vec3 ToLocal(const Vec3& v) const;

    Vec3& operator[](int32 i);
    Vec3 operator[](int32 i) const;

    Vec3 x, y, z;
};

inline Frame Frame::FromXZ(const Vec3& x, const Vec3& z)
{
    return Frame(x, Cross(z, x), z);
}

inline Frame Frame::FromXY(const Vec3& x, const Vec3& y)
{
    return Frame(x, y, Cross(x, y));
}

inline Frame Frame::FromX(const Vec3& x)
{
    Vec3 y, z;
    CoordinateSystem(x, &y, &z);
    return Frame(x, y, z);
}

inline Frame Frame::FromY(const Vec3& y)
{
    Vec3 z, x;
    CoordinateSystem(y, &z, &x);
    return Frame(x, y, z);
}

inline Frame Frame::FromZ(const Vec3& z)
{
    Vec3 x, y;
    CoordinateSystem(z, &x, &y);
    return Frame(x, y, z);
}

inline Frame::Frame(const Vec3& n)
    : z{ n }
{
    CoordinateSystem(n, &x, &y);
}

inline Frame::Frame(const Vec3& x, const Vec3& y, const Vec3& z)
    : x{ x }
    , y{ y }
    , z{ z }
{
}

inline Vec3& Frame::operator[](int32 i)
{
    return (&x)[i];
}

inline Vec3 Frame::operator[](int32 i) const
{
    return (&x)[i];
}

inline Vec3 Frame::FromLocal(const Vec3& v) const
{
    return v.x * x + v.y * y + v.z * z;
}

inline Vec3 Frame::ToLocal(const Vec3& v) const
{
    return Vec3(Dot(v, x), Dot(v, y), Dot(v, z));
}

} // namespace bulbit
