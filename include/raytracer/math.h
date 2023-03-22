// Simple linear math library
// Highly inspired by Box2d, glm math codes

#pragma once

#include <float.h>
#include <limits>
#include <math.h>
#include <random>
#include <stdint.h>

#include "types.h"

#define Real double

namespace spt
{

constexpr Real pi = Real(3.14159265358979323846);
constexpr Real infinity = std::numeric_limits<Real>::infinity();
constexpr Real epsilon = std::numeric_limits<Real>::epsilon();

struct Vec2;
struct Vec3;
struct Vec4;
struct Quat;
struct Mat2;
struct Mat3;
struct Mat4;
struct Transform;

enum Identity
{
    identity
};

inline Real Prand()
{
    static thread_local std::minstd_rand prng;

    return Real(prng()) / std::minstd_rand::max();
}

inline Real Prand(Real min, Real max)
{
    return min + (max - min) * Prand();
}

inline Real DegToRad(Real deg)
{
    return Real(deg * pi / 180.0);
}

inline Real RadToDeg(Real rad)
{
    return Real(rad / pi * 180.0);
}

inline Real Rand()
{
    static thread_local std::uniform_real_distribution<Real> distribution(0.0, 1.0);
    static thread_local std::mt19937 generator;

    return distribution(generator);
}

inline Real Rand(Real min, Real max)
{
    return min + (max - min) * Rand();
}

struct Vec2
{
    Real x, y;

    Vec2() = default;

    explicit constexpr Vec2(Real _v)
        : x{ _v }
        , y{ _v }
    {
    }

    constexpr Vec2(Real _x, Real _y)
        : x{ _x }
        , y{ _y }
    {
    }

    void SetZero()
    {
        x = Real(0.0);
        y = Real(0.0);
    }

    void Set(Real _x, Real _y)
    {
        x = _x;
        y = _y;
    }

    Real operator[](int32 i) const
    {
        return (&x)[i];
    }

    Real& operator[](int32 i)
    {
        return (&x)[i];
    }

    Vec2 operator-() const
    {
        return Vec2{ -x, -y };
    }

    void operator+=(Real s)
    {
        x += s;
        y += s;
    }

    void operator+=(const Vec2& v)
    {
        x += v.x;
        y += v.y;
    }

    void operator-=(Real s)
    {
        x -= s;
        y -= s;
    }

    void operator-=(const Vec2& v)
    {
        x -= v.x;
        y -= v.y;
    }

    void operator*=(Real s)
    {
        x *= s;
        y *= s;
    }

    void operator*=(const Vec2& v)
    {
        x *= v.x;
        y *= v.y;
    }

    void operator/=(Real s)
    {
        operator*=(1.0 / s);
    }

    Real Length() const
    {
        return sqrt(x * x + y * y);
    }

    Real Length2() const
    {
        return x * x + y * y;
    }

    // Optimized to not check length == 0
    Real Normalize()
    {
        Real length = Length();
        Real invLength = Real(1.0) / length;
        x *= invLength;
        y *= invLength;

        return length;
    }

    // Optimized to not check length == 0
    Vec2 Normalized() const
    {
        Real invLength = Real(1.0) / Length();

        return Vec2{ x * invLength, y * invLength };
    }

    // Get the skew vector such that dot(skew_vec, other) == cross(vec, other)
    // return cross(1, *this);
    Vec2 Skew() const
    {
        return Vec2{ -y, x };
    }
};

struct Vec3
{
    Real x, y, z;

    Vec3() = default;

    inline static Vec3 Random()
    {
        return Vec3{ Rand(), Rand(), Rand() };
    }

    inline static Vec3 Random(Real min, Real max)
    {
        return Vec3{ Rand(min, max), Rand(min, max), Rand(min, max) };
    }

    explicit constexpr Vec3(Real _v)
        : x{ _v }
        , y{ _v }
        , z{ _v }
    {
    }

    constexpr Vec3(Real _x, Real _y, Real _z)
        : x{ _x }
        , y{ _y }
        , z{ _z }
    {
    }

    Vec3(const Vec2& _v)
        : x{ _v.x }
        , y{ _v.y }
        , z{ Real(0.0) }
    {
    }

    void SetZero()
    {
        x = Real(0.0);
        y = Real(0.0);
        z = Real(0.0);
    }

    void Set(Real _x, Real _y, Real _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    Real operator[](int32 i) const
    {
        return (&x)[i];
    }

    Real& operator[](int32 i)
    {
        return (&x)[i];
    }

    Vec3 operator-() const
    {
        return Vec3{ -x, -y, -z };
    }

    void operator+=(Real s)
    {
        x += s;
        y += s;
        z += s;
    }

    void operator+=(const Vec3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    void operator-=(Real s)
    {
        x -= s;
        y -= s;
        z -= s;
    }

    void operator-=(const Vec3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    void operator*=(Real s)
    {
        x *= s;
        y *= s;
        z *= s;
    }

    void operator*=(const Vec3& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
    }

    void operator/=(Real s)
    {
        operator*=(Real(1.0) / s);
    }

    Real Length() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    Real Length2() const
    {
        return x * x + y * y + z * z;
    }

    Real Normalize()
    {
        Real length = Length();
        Real invLength = Real(1.0) / length;
        x *= invLength;
        y *= invLength;
        z *= invLength;

        return length;
    }

    Vec3 Normalized() const
    {
        Real length = Length();
        if (length < epsilon)
        {
            return Vec3{ Real(0.0), Real(0.0), Real(0.0) };
        }

        Real invLength = Real(1.0) / length;

        return Vec3{ x * invLength, y * invLength, z * invLength };
    }
};

struct Vec4
{
    Real x, y, z, w;

    Vec4() = default;

    constexpr Vec4(Real _v, Real _w)
        : x{ _v }
        , y{ _v }
        , z{ _v }
        , w{ _w }
    {
    }

    constexpr Vec4(Real _x, Real _y, Real _z, Real _w)
        : x{ _x }
        , y{ _y }
        , z{ _z }
        , w{ _w }
    {
    }

    constexpr Vec4(const Vec3& _v, Real _w)
        : x{ _v.x }
        , z{ _v.z }
        , y{ _v.y }
        , w{ _w }
    {
    }

    void SetZero()
    {
        x = Real(0.0);
        y = Real(0.0);
        z = Real(0.0);
        w = Real(0.0);
    }

    void Set(Real _x, Real _y, Real _z, Real _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    Vec4 operator-() const
    {
        return Vec4{ -x, -y, -z, -w };
    }

    void operator+=(Real s)
    {
        x += s;
        y += s;
        z += s;
        w += s;
    }

    void operator+=(const Vec4& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
    }

    void operator-=(Real s)
    {
        x -= s;
        y -= s;
        z -= s;
        w -= s;
    }

    void operator-=(const Vec4& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
    }

    void operator*=(Real s)
    {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
    }

    void operator*=(const Vec4& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        w *= v.w;
    }

    void operator/=(Real s)
    {
        operator*=(Real(1.0) / s);
    }

    Real operator[](int32 i) const
    {
        return (&x)[i];
    }

    Real& operator[](int32 i)
    {
        return (&x)[i];
    }

    Real Length() const
    {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    Real Length2() const
    {
        return x * x + y * y + z * z + w * w;
    }

    Real Normalize()
    {
        Real length = Length();
        Real invLength = Real(1.0) / length;
        x *= invLength;
        y *= invLength;
        z *= invLength;
        w *= invLength;

        return length;
    }
};

// Column major matrices

struct Mat2
{
    Vec2 ex, ey;

    Mat2() = default;

    Mat2(Identity)
        : Mat2(Real(1.0))
    {
    }

    explicit Mat2(Real v)
    {
        // clang-format off
        ex.x = v;       ey.x = Real(0.0);
        ex.y = Real(0.0);    ey.y = v;
        // clang-format on
    }

    constexpr Mat2(const Vec2& c1, const Vec2& c2)
        : ex{ c1 }
        , ey{ c2 }
    {
    }

    Vec2& operator[](int32 i)
    {
        return (&ex)[i];
    }

    void SetIdentity()
    {
        // clang-format off
        ex.x = Real(1.0);    ey.x = Real(0.0);
        ex.y = Real(0.0);    ey.y = Real(1.0);
        // clang-format on
    }

    void SetZero()
    {
        // clang-format off
        ex.x = Real(0.0);    ey.x = Real(0.0);
        ex.y = Real(0.0);    ey.y = Real(0.0);
        // clang-format on
    }

    Mat2 GetTranspose()
    {
        Mat2 t;

        // clang-format off
        t.ex.x = ex.x;    t.ey.x = ex.y;
        t.ex.y = ey.x;    t.ey.y = ey.y;
        // clang-format on

        return t;
    }

    Mat2 GetInverse() const
    {
        Mat2 t;

        Real det = ex.x * ey.y - ey.x * ex.y;
        if (det != Real(0.0))
        {
            det = Real(1.0) / det;
        }

        t.ex.x = det * ey.y;
        t.ey.x = -det * ey.x;
        t.ex.y = -det * ex.y;
        t.ey.y = det * ex.x;

        return t;
    }

    Real GetDeterminant() const
    {
        return ex.x * ey.y - ey.x * ex.y;
    }
};

struct Mat3
{
    Vec3 ex, ey, ez;

    Mat3() = default;

    Mat3(Identity)
        : Mat3(Real(1.0))
    {
    }

    explicit Mat3(Real v)
    {
        // clang-format off
        ex.x = v;       ey.x = Real(0.0);    ez.x = Real(0.0);
        ex.y = Real(0.0);    ey.y = v;       ez.y = Real(0.0);
        ex.z = Real(0.0);    ey.z = Real(0.0);    ez.z = v;
        // clang-format on
    }

    constexpr Mat3(const Vec3& c1, const Vec3& c2, const Vec3& c3)
        : ex{ c1 }
        , ey{ c2 }
        , ez{ c3 }
    {
    }

    Mat3(const Quat& q);

    Vec3& operator[](int32 i)
    {
        return (&ex)[i];
    }

    void SetIdentity()
    {
        // clang-format off
        ex.x = Real(1.0);    ey.x = Real(0.0);    ez.x = Real(0.0);
        ex.y = Real(0.0);    ey.y = Real(1.0);    ez.y = Real(0.0);
        ex.z = Real(0.0);    ey.z = Real(0.0);    ez.z = Real(1.0);
        // clang-format on
    }

    void SetZero()
    {
        // clang-format off
        ex.x = Real(0.0);    ey.x = Real(0.0);    ez.x = Real(0.0);
        ex.y = Real(0.0);    ey.y = Real(0.0);    ez.y = Real(0.0);
        ex.z = Real(0.0);    ey.z = Real(0.0);    ez.z = Real(0.0);
        // clang-format on
    }

    Mat3 GetTranspose()
    {
        Mat3 t;

        // clang-format off
        t.ex.x = ex.x;    t.ey.x = ex.y;    t.ez.x = ex.z;
        t.ex.y = ey.x;    t.ey.y = ey.y;    t.ez.y = ey.z;
        t.ex.z = ez.x;    t.ey.z = ez.y;    t.ez.z = ez.z;
        // clang-format on

        return t;
    }

    Mat3 GetInverse() const;
    Mat3 Scale(Real x, Real y);
    Mat3 Rotate(Real z);
    Mat3 Translate(Real x, Real y);
    Mat3 Translate(const Vec2& v);
};

struct Mat4
{
    Vec4 ex, ey, ez, ew;

    Mat4() = default;

    Mat4(Identity)
        : Mat4(Real(1.0))
    {
    }

    explicit Mat4(Real _v)
    {
        // clang-format off
        ex.x = _v;           ey.x = Real(0.0);    ez.x = Real(0.0);    ew.x = Real(0.0);
        ex.y = Real(0.0);    ey.y = _v;           ez.y = Real(0.0);    ew.y = Real(0.0);
        ex.z = Real(0.0);    ey.z = Real(0.0);    ez.z = _v;           ew.z = Real(0.0);
        ex.w = Real(0.0);    ey.w = Real(0.0);    ez.w = Real(0.0);    ew.w = _v;
        // clang-format on
    }

    constexpr Mat4(const Vec4& c1, const Vec4& c2, const Vec4& c3, const Vec4& c4)
        : ex{ c1 }
        , ey{ c2 }
        , ez{ c3 }
        , ew{ c4 }
    {
    }

    Mat4(const Mat3& r, const Vec3& p)
        : ex{ r.ex, Real(0.0) }
        , ey{ r.ey, Real(0.0) }
        , ez{ r.ez, Real(0.0) }
        , ew{ p, Real(1.0) }
    {
    }

    Mat4(const Transform& t);

    Vec4& operator[](int32 i)
    {
        return (&ex)[i];
    }

    void SetIdentity()
    {
        // clang-format off
        ex.x = Real(1.0);    ey.x = Real(0.0);    ez.x = Real(0.0);    ew.x = Real(0.0);
        ex.y = Real(0.0);    ey.y = Real(1.0);    ez.y = Real(0.0);    ew.y = Real(0.0);
        ex.z = Real(0.0);    ey.z = Real(0.0);    ez.z = Real(1.0);    ew.z = Real(0.0);
        ex.w = Real(0.0);    ey.w = Real(0.0);    ez.w = Real(0.0);    ew.w = Real(1.0);
        // clang-format on
    }

    void SetZero()
    {
        // clang-format off
        ex.x = Real(0.0);    ey.x = Real(0.0);    ez.x = Real(0.0);    ew.x = Real(0.0);
        ex.y = Real(0.0);    ey.y = Real(0.0);    ez.y = Real(0.0);    ew.y = Real(0.0);
        ex.z = Real(0.0);    ey.z = Real(0.0);    ez.z = Real(0.0);    ew.z = Real(0.0);
        ex.w = Real(0.0);    ey.w = Real(0.0);    ez.w = Real(0.0);    ew.w = Real(0.0);
        // clang-format on
    }

    Mat4 GetTranspose()
    {
        Mat4 t;

        // clang-format off
        t.ex.x = ex.x;    t.ey.x = ex.y;    t.ez.x = ex.z;    t.ew.x = ex.w;
        t.ex.y = ey.x;    t.ey.y = ey.y;    t.ez.y = ey.z;    t.ew.y = ey.w;
        t.ex.z = ez.x;    t.ey.z = ez.y;    t.ez.z = ez.z;    t.ew.z = ez.w;
        t.ex.w = ew.x;    t.ey.w = ew.y;    t.ez.w = ew.z;    t.ew.w = ew.w;
        // clang-format on

        return t;
    }

    Mat4 GetInverse();
    Mat4 Scale(Real x, Real y, Real z);
    Mat4 Rotate(Real x, Real y, Real z);
    Mat4 Translate(Real x, Real y, Real z);
    Mat4 Translate(const Vec3& v);
};

struct Quat
{
    Quat() = default;

    Quat(Identity)
        : Quat(Real(1.0))
    {
    }

    Quat(Real _x, Real _y, Real _z, Real _w)
        : x{ _x }
        , y{ _y }
        , z{ _z }
        , w{ _w }
    {
    }

    explicit Quat(Real _w)
        : x{ Real(0.0) }
        , y{ Real(0.0) }
        , z{ Real(0.0) }
        , w{ _w }
    {
    }

    Quat(const Mat3& m);

    Quat(const Vec3& dir, const Vec3& up);

    // Axis must be normalized
    Quat(Real angle, const Vec3& unitAxis)
    {
        Real halfAngle = angle * Real(0.5);

        Real s = sin(halfAngle);
        x = unitAxis.x * s;
        y = unitAxis.y * s;
        z = unitAxis.z * s;
        w = cos(halfAngle);
    }

    Quat operator-()
    {
        return Quat{ -x, -y, -z, -w };
    }

    Quat operator*(Real s) const
    {
        return Quat{ x * s, y * s, z * s, w * s };
    }

    bool IsIdentity() const
    {
        return x == Real(0.0) && y == Real(0.0) && z == Real(0.0) && w == Real(1.0);
    }

    // Magnitude
    Real Length() const
    {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    Real Length2() const
    {
        return x * x + y * y + z * z + w * w;
    }

    Real Normalize()
    {
        Real length = Length();
        Real invLength = Real(1.0) / length;
        x *= invLength;
        y *= invLength;
        z *= invLength;
        w *= invLength;

        return length;
    }

    Quat Normalized() const
    {
        Real length = Length();
        if (length < epsilon)
        {
            return Quat{ Real(0.0), Real(0.0), Real(0.0), Real(0.0) };
        }

        Real invLength = Real(1.0) / length;

        return Quat{ x * invLength, y * invLength, z * invLength, w * invLength };
    }

    Quat GetConjugate() const
    {
        return Quat{ -x, -y, -z, w };
    }

    Vec3 GetImaginaryPart() const
    {
        return Vec3{ x, y, z };
    }

    // Optimized qvq'
    Vec3 Rotate(const Vec3& v) const
    {
        Real vx = Real(2.0) * v.x;
        Real vy = Real(2.0) * v.y;
        Real vz = Real(2.0) * v.z;
        Real w2 = w * w - Real(0.5);

        Real dot2 = (x * vx + y * vy + z * vz);

        return Vec3((vx * w2 + (y * vz - z * vy) * w + x * dot2), (vy * w2 + (z * vx - x * vz) * w + y * dot2),
                    (vz * w2 + (x * vy - y * vx) * w + z * dot2));
    }

    Vec3 RotateInv(const Vec3& v) const
    {
        Real vx = Real(2.0) * v.x;
        Real vy = Real(2.0) * v.y;
        Real vz = Real(2.0) * v.z;
        Real w2 = w * w - Real(0.5);

        Real dot2 = (x * vx + y * vy + z * vz);

        return Vec3((vx * w2 - (y * vz - z * vy) * w + x * dot2), (vy * w2 - (z * vx - x * vz) * w + y * dot2),
                    (vz * w2 - (x * vy - y * vx) * w + z * dot2));
    }

    void SetIdentity()
    {
        x = Real(0.0);
        y = Real(0.0);
        z = Real(0.0);
        w = Real(1.0);
    }

    // Computes rotation of x-axis
    Vec3 GetBasisX() const
    {
        Real x2 = x * Real(2.0);
        Real w2 = w * Real(2.0);

        return Vec3((w * w2) - Real(1.0) + x * x2, (z * w2) + y * x2, (-y * w2) + z * x2);
    }

    // Computes rotation of y-axis
    Vec3 GetBasisY() const
    {
        Real y2 = y * Real(2.0);
        Real w2 = w * Real(2.0);

        return Vec3((-z * w2) + x * y2, (w * w2) - Real(1.0) + y * y2, (x * w2) + z * y2);
    }

    // Computes rotation of z-axis
    Vec3 GetBasisZ() const
    {
        Real z2 = z * Real(2.0);
        Real w2 = w * Real(2.0);

        return Vec3((y * w2) + x * z2, (-x * w2) + y * z2, (w * w2) - Real(1.0) + z * z2);
    }

    Real x, y, z, w;
};

inline Real Dot(const Vec2& a, const Vec2& b)
{
    return a.x * b.x + a.y * b.y;
}

inline Real Cross(const Vec2& a, const Vec2& b)
{
    return a.x * b.y - a.y * b.x;
}

inline Vec2 Cross(Real s, const Vec2& v)
{
    return Vec2{ -s * v.y, s * v.x };
}

inline Vec2 Cross(const Vec2& v, Real s)
{
    return Vec2{ s * v.y, -s * v.x };
}

inline Vec2 operator+(const Vec2& a, const Vec2& b)
{
    return Vec2{ a.x + b.x, a.y + b.y };
}

inline Vec2 operator-(const Vec2& a, const Vec2& b)
{
    return Vec2{ a.x - b.x, a.y - b.y };
}

inline Vec2 operator*(const Vec2& v, Real s)
{
    return Vec2{ v.x * s, v.y * s };
}

inline Vec2 operator*(Real s, const Vec2& v)
{
    return operator*(v, s);
}

inline Vec2 operator*(const Vec2& a, const Vec2& b)
{
    return Vec2{ a.x * b.x, a.y * b.y };
}

inline Vec2 operator/(const Vec2& v, Real s)
{
    return v * (Real(1.0) / s);
}

inline bool operator==(const Vec2& a, const Vec2& b)
{
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const Vec2& a, const Vec2& b)
{
    return a.x != b.x || a.y != b.y;
}

inline Real Dist(const Vec2& a, const Vec2& b)
{
    return (a - b).Length();
}

inline Real Dist2(const Vec2& a, const Vec2& b)
{
    return (a - b).Length2();
}

inline Real Length(const Vec2& v)
{
    return v.Length();
}

inline Real Length2(const Vec2& v)
{
    return v.Length2();
}

inline Vec2 Normalize(const Vec2& v)
{
    return v.Normalized();
}

// Vec2 functions end

// Vec3 functions begin

inline Real Dot(const Vec3& a, const Vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 Cross(const Vec3& a, const Vec3& b)
{
    return Vec3{ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}

inline Vec3 operator+(const Vec3& a, const Vec3& b)
{
    return Vec3{ a.x + b.x, a.y + b.y, a.z + b.z };
}

inline Vec3 operator-(const Vec3& a, const Vec3& b)
{
    return Vec3{ a.x - b.x, a.y - b.y, a.z - b.z };
}

inline Vec3 operator*(const Vec3& v, Real s)
{
    return Vec3{ v.x * s, v.y * s, v.z * s };
}

inline Vec3 operator*(Real s, const Vec3& v)
{
    return operator*(v, s);
}

inline Vec3 operator*(const Vec3& a, const Vec3& b)
{
    return Vec3{ a.x * b.x, a.y * b.y, a.z * b.z };
}

inline Vec3 operator/(const Vec3& v, Real s)
{
    return v * (Real(1.0) / s);
}

inline Vec3 operator/(Real s, const Vec3& v)
{
    return (Real(1.0) / s) * v;
}

inline bool operator==(const Vec3& a, const Vec3& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline bool operator!=(const Vec3& a, const Vec3& b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z;
}

inline Real Dist(const Vec3& a, const Vec3& b)
{
    return (b - a).Length();
}

inline Real Dist2(const Vec3& a, const Vec3& b)
{
    return (b - a).Length2();
}

// Vec3 functions end

// Vec4 functions begin

inline Real Dot(const Vec4& a, const Vec4& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline Vec4 operator+(const Vec4& a, const Vec4& b)
{
    return Vec4{ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

inline Vec4 operator-(const Vec4& a, const Vec4& b)
{
    return Vec4{ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

inline bool operator==(const Vec4& a, const Vec4& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

inline bool operator!=(const Vec4& a, const Vec4& b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}

// Vec4 functions end

// Quat functions begin

inline bool operator==(const Quat& a, const Quat& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

inline Real Dot(const Quat& a, const Quat& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// Quaternion multiplication
inline Quat operator*(const Quat& a, const Quat& b)
{
    // clang-format off
    return Quat(a.w * b.x + b.w * a.x + a.y * b.z - b.y * a.z,
                a.w * b.y + b.w * a.y + a.z * b.x - b.z * a.x,
                a.w * b.z + b.w * a.z + a.x * b.y - b.x * a.y,
                a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);
    // clang-format on
}

inline Quat operator+(const Quat& a, const Quat& b)
{
    return Quat(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

inline Quat operator-(const Quat& a, const Quat& b)
{
    return Quat(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

// Compute angle between two quaternions
inline Real Angle(const Quat& a, const Quat& b)
{
    return acos(Dot(a, b)) * Real(2.0);
}

// Quat functions end

// Mat2 functions begin

inline Mat2 operator+(const Mat2& a, const Mat2& b)
{
    return Mat2{ a.ex + b.ex, a.ey + b.ey };
}

// M * V
inline Vec2 Mul(const Mat2& m, const Vec2& v)
{
    return Vec2{
        m.ex.x * v.x + m.ey.x * v.y,
        m.ex.y * v.x + m.ey.y * v.y,
    };
}

inline Vec2 operator*(const Mat2& m, const Vec2& v)
{
    return Vec2{
        m.ex.x * v.x + m.ey.x * v.y,
        m.ex.y * v.x + m.ey.y * v.y,
    };
}

// M^T * V
inline Vec2 MulT(const Mat2& m, const Vec2& v)
{
    return Vec2{ Dot(m.ex, v), Dot(m.ey, v) };
}

// A * B
inline Mat2 Mul(const Mat2& a, const Mat2& b)
{
    return Mat2{ a * b.ex, a * b.ey };
}

inline Mat2 operator*(const Mat2& a, const Mat2& b)
{
    return Mat2{ a * b.ex, a * b.ey };
}

// A^T * B
inline Mat2 MulT(const Mat2& a, const Mat2& b)
{
    Vec2 c1{ Dot(a.ex, b.ex), Dot(a.ey, b.ex) };
    Vec2 c2{ Dot(a.ex, b.ey), Dot(a.ey, b.ey) };

    return Mat2{ c1, c2 };
}

// Mat2 functions end

// Mat3 functions begin

// M * V
inline Vec3 Mul(const Mat3& m, const Vec3& v)
{
    return Vec3{
        m.ex.x * v.x + m.ey.x * v.y + m.ez.x * v.z,
        m.ex.y * v.x + m.ey.y * v.y + m.ez.y * v.z,
        m.ex.z * v.x + m.ey.z * v.y + m.ez.z * v.z,
    };
}

inline Vec3 operator*(const Mat3& m, const Vec3& v)
{
    return Vec3{
        m.ex.x * v.x + m.ey.x * v.y + m.ez.x * v.z,
        m.ex.y * v.x + m.ey.y * v.y + m.ez.y * v.z,
        m.ex.z * v.x + m.ey.z * v.y + m.ez.z * v.z,
    };
}

// M^T * V
inline Vec3 MulT(const Mat3& m, const Vec3& v)
{
    return Vec3{ Dot(m.ex, v), Dot(m.ey, v), Dot(m.ez, v) };
}

// A * B
inline Mat3 Mul(const Mat3& a, const Mat3& b)
{
    return Mat3{ a * b.ex, a * b.ey, a * b.ez };
}

inline Mat3 operator*(const Mat3& a, const Mat3& b)
{
    return Mat3{ a * b.ex, a * b.ey, a * b.ez };
}

// A^T * B
inline Mat3 MulT(const Mat3& a, const Mat3& b)
{
    Vec3 c1{ Dot(a.ex, b.ex), Dot(a.ey, b.ex), Dot(a.ez, b.ex) };
    Vec3 c2{ Dot(a.ex, b.ey), Dot(a.ey, b.ey), Dot(a.ez, b.ey) };
    Vec3 c3{ Dot(a.ex, b.ez), Dot(a.ey, b.ez), Dot(a.ez, b.ez) };

    return Mat3{ c1, c2, c3 };
}

// Mat3 functions end

// Mat4 functions begin

// M * V
inline Vec4 Mul(const Mat4& m, const Vec4& v)
{
    return Vec4{
        m.ex.x * v.x + m.ey.x * v.y + m.ez.x * v.z + m.ew.x * v.w,
        m.ex.y * v.x + m.ey.y * v.y + m.ez.y * v.z + m.ew.y * v.w,
        m.ex.z * v.x + m.ey.z * v.y + m.ez.z * v.z + m.ew.z * v.w,
        m.ex.w * v.x + m.ey.w * v.y + m.ez.w * v.z + m.ew.w * v.w,
    };
}

inline Vec4 operator*(const Mat4& m, const Vec4& v)
{
    return Vec4{
        m.ex.x * v.x + m.ey.x * v.y + m.ez.x * v.z + m.ew.x * v.w,
        m.ex.y * v.x + m.ey.y * v.y + m.ez.y * v.z + m.ew.y * v.w,
        m.ex.z * v.x + m.ey.z * v.y + m.ez.z * v.z + m.ew.z * v.w,
        m.ex.w * v.x + m.ey.w * v.y + m.ez.w * v.z + m.ew.w * v.w,
    };
}

// M^T * V
inline Vec4 MulT(const Mat4& m, const Vec4& v)
{
    return Vec4{ Dot(m.ex, v), Dot(m.ey, v), Dot(m.ez, v), Dot(m.ew, v) };
}

// A * B
inline Mat4 Mul(const Mat4& a, const Mat4& b)
{
    return Mat4{ a * b.ex, a * b.ey, a * b.ez, a * b.ew };
}

inline Mat4 operator*(const Mat4& a, const Mat4& b)
{
    return Mat4{ a * b.ex, a * b.ey, a * b.ez, a * b.ew };
}

// A^T * B
inline Mat4 MulT(const Mat4& a, const Mat4& b)
{
    Vec4 c1{ Dot(a.ex, b.ex), Dot(a.ey, b.ex), Dot(a.ez, b.ex), Dot(a.ew, b.ex) };
    Vec4 c2{ Dot(a.ex, b.ey), Dot(a.ey, b.ey), Dot(a.ez, b.ey), Dot(a.ew, b.ey) };
    Vec4 c3{ Dot(a.ex, b.ez), Dot(a.ey, b.ez), Dot(a.ez, b.ez), Dot(a.ew, b.ez) };
    Vec4 c4{ Dot(a.ex, b.ew), Dot(a.ey, b.ew), Dot(a.ez, b.ew), Dot(a.ew, b.ew) };

    return Mat4{ c1, c2, c3, c4 };
}

inline Mat4 Orth(Real left, Real right, Real bottom, Real top, Real zNear, Real zFar)
{
    Mat4 t{ Real(1.0) };

    // Scale
    t.ex.x = Real(2.0) / (right - left);
    t.ey.y = Real(2.0) / (top - bottom);
    t.ez.z = Real(2.0) / (zFar - zNear);

    // Translation
    t.ew.x = -(right + left) / (right - left);
    t.ew.y = -(top + bottom) / (top - bottom);
    t.ew.z = -(zFar + zNear) / (zFar - zNear);

    return t;
}

// Mat4 functions end

template <typename T>
inline T Abs(T a)
{
    return a > T(0) ? a : -a;
}

inline Vec2 Abs(const Vec2& a)
{
    return Vec2(Abs(a.x), Abs(a.y));
}

template <typename T>
inline T Min(T a, T b)
{
    return a < b ? a : b;
}

template <typename T>
inline T Max(T a, T b)
{
    return a > b ? a : b;
}

inline Vec2 Min(const Vec2& a, const Vec2& b)
{
    return Vec2(Min(a.x, b.x), Min(a.y, b.y));
}

inline Vec2 Max(const Vec2& a, const Vec2& b)
{
    return Vec2(Max(a.x, b.x), Max(a.y, b.y));
}

inline Vec3 Min(const Vec3& a, const Vec3& b)
{
    return Vec3(Min(a.x, b.x), Min(a.y, b.y), Min(a.z, b.z));
}

inline Vec3 Max(const Vec3& a, const Vec3& b)
{
    return Vec3(Max(a.x, b.x), Max(a.y, b.y), Max(a.z, b.z));
}

template <typename T>
inline T Clamp(T v, T _min, T _max)
{
    return Max(_min, Min(v, _max));
}

inline Vec2 Clamp(const Vec2& a, const Vec2& _min, const Vec2& _max)
{
    return Max(_min, Min(a, _max));
}

template <typename T>
inline T Lerp(const T& a, const T& b, Real t)
{
    return a * (Real(1.0) - t) + b * t;
}

template <typename T>
inline T Slerp(const T& start, const T& end, Real percent)
{
    Real dot = Clamp(Dot(start, end), -Real(1.0), Real(1.0));
    Real angle = acosf(dot) * percent;

    T rv = end - start * dot;
    rv.Normalize();

    return start * cos(angle) + rv * sin(angle);
}

template <typename T>
inline T Project(const T& v, const T& n)
{
    return v - n * Dot(v, n);
}

template <typename T>
inline T Reflect(const T& v, const T& n)
{
    return v - 2 * Dot(v, n) * n;
}

template <typename T>
T Refract(const T& uv, const T& n, Real etai_over_etat)
{
    auto cos_theta = fmin(Dot(-uv, n), Real(1.0));
    T r_out_perp = etai_over_etat * (uv + cos_theta * n);
    T r_out_parallel = -sqrt(fabs(Real(1.0) - r_out_perp.Length2())) * n;

    return r_out_perp + r_out_parallel;
}

inline Vec3 PolarToCart(Real theta, Real phi, Real r)
{
    Real x = cos(phi) * sin(theta);
    Real y = sin(phi) * sin(theta);
    Real z = cos(theta);

    return Vec3{ x, y, z } * r;
}

inline Vec3 RandomUnitVector()
{
    Real r1 = Rand();
    Real r2 = Rand();
    Real x = cos(2 * pi * r1) * 2 * sqrt(r2 * (1 - r2));
    Real y = sin(2 * pi * r1) * 2 * sqrt(r2 * (1 - r2));
    Real z = 1 - 2 * r2;

    return Vec3{ x, y, z };
}

inline Vec3 RandomToSphere(double radius, double distance_squared)
{
    double r1 = Rand();
    double r2 = Rand();
    double z = 1.0 + r2 * (sqrt(1.0 - radius * radius / distance_squared) - 1.0);

    double phi = 2.0 * pi * r1;
    double x = cos(phi) * sqrt(1.0 - z * z);
    double y = sin(phi) * sqrt(1.0 - z * z);

    return Vec3{ x, y, z };
}

// z > 0
inline Vec3 RandomCosineDirection()
{
    Real r1 = Rand();
    Real r2 = Rand();
    Real z = sqrt(1 - r2);

    Real phi = 2 * pi * r1;
    Real x = cos(phi) * sqrt(r2);
    Real y = sin(phi) * sqrt(r2);

    return Vec3{ x, y, z };
}

inline Vec3 RandomInUnitSphere()
{
    Real r = Rand();

    return RandomUnitVector() * r;
}

inline Vec3 RandomInHemisphere(const Vec3& normal)
{
    Vec3 in_unit_sphere = RandomInUnitSphere();

    if (Dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
    {
        return in_unit_sphere;
    }
    else
    {
        return -in_unit_sphere;
    }
}

inline Vec3 RandomInUnitDisk()
{
    return Vec3{ Rand(-1.0, 1.0), Rand(-1.0, 1.0), 0.0 };
}

} // namespace spt