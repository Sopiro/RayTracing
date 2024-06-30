#pragma once

#include "common.h"

namespace bulbit
{

constexpr inline Float CosTheta(const Vec3& w)
{
    return w.z;
}

constexpr inline Float Cos2Theta(const Vec3& w)
{
    return Sqr(w.z);
}

inline Float AbsCosTheta(const Vec3& w)
{
    return std::abs(w.z);
}

constexpr inline Float Sin2Theta(const Vec3& w)
{
    return std::max(Float(0), 1 - Cos2Theta(w));
}

inline Float SinTheta(const Vec3& w)
{
    return std::sqrt(Sin2Theta(w));
}

inline Float TanTheta(const Vec3& w)
{
    return SinTheta(w) / CosTheta(w);
}

inline Float Tan2Theta(const Vec3& w)
{
    return Sin2Theta(w) / Cos2Theta(w);
}

inline Float CosPhi(const Vec3& w)
{
    Float sin_theta = SinTheta(w);
    return (sin_theta == 0) ? 1 : Clamp(w.x / sin_theta, -1, 1);
}

inline Float SinPhi(const Vec3& w)
{
    Float sin_theta = SinTheta(w);
    return (sin_theta == 0) ? 0 : Clamp(w.y / sin_theta, -1, 1);
}

inline Float SphericalTheta(const Vec3& v)
{
    return std::acos(Clamp(v.y, -1, 1));
}

inline Float SphericalPhi(const Vec3& v)
{
    Float r = std::atan2(v.z, v.x);
    return r < 0 ? r + two_pi : r;
}

inline Vec3 SphericalDirection(Float theta, Float phi)
{
    Float sin_theta = std::sin(theta);
    return Vec3(std::cos(phi) * sin_theta, std::cos(theta), std::sin(phi) * sin_theta);
}

constexpr inline Vec3 SphericalDirection(Float sin_theta, Float cos_theta, Float phi)
{
    return Vec3(std::cos(phi) * sin_theta, cos_theta, std::sin(phi) * sin_theta);
}

constexpr inline Vec3 SphericalDirection(Float sin_theta, Float cos_theta, Float sin_phi, Float cos_phi)
{
    return Vec3(cos_phi * sin_theta, cos_theta, sin_phi * sin_theta);
}

// Assume it's a standard shading coordinate system
constexpr inline bool SameHemisphere(const Vec3& a, const Vec3& b)
{
    return a.z * b.z > 0;
}

inline void CoordinateSystem(const Vec3& v1, Vec3* v2, Vec3* v3)
{
    Float sign = std::copysign(1.0f, v1.z);
    Float a = -1 / (sign + v1.z);
    Float b = v1.x * v1.y * a;
    *v2 = Vec3(1 + sign * (v1.x * v1.x) * a, sign * b, -sign * v1.x);
    *v3 = Vec3(b, sign + (v1.y * v1.y) * a, -v1.y);
}

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
