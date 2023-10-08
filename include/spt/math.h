// Simple linear math library
// Highly inspired by Box2d, glm math codes

#pragma once

#include <cfloat>
#include <cmath>
#include <cstdint>
#include <limits>

#include "types.h"

namespace spt
{

using Float = double;

constexpr Float pi = Float(3.14159265358979323846);
constexpr Float two_pi = Float(2.0 * 3.14159265358979323846);
constexpr Float four_pi = Float(4.0 * 3.14159265358979323846);
constexpr Float inv_pi = Float(1.0 / 3.14159265358979323846);
constexpr Float inv_two_pi = Float(1.0 / (2.0 * 3.14159265358979323846));
constexpr Float inv_four_pi = Float(1.0 / (4.0 * 3.14159265358979323846));
constexpr Float epsilon = std::numeric_limits<Float>::epsilon();
constexpr Float infinity = std::numeric_limits<Float>::infinity();

struct Vec2;
struct Vec3;
struct Vec4;
struct Quat;
struct Mat2;
struct Mat3;
struct Mat4;
struct Transform;

using Point2 = Vec2;
using Color = Vec3;
using Point3 = Vec3;

enum Identity
{
    identity
};

struct Vec2
{
    Float x, y;

    Vec2() = default;

    explicit constexpr Vec2(Float _v)
        : x{ _v }
        , y{ _v }
    {
    }

    constexpr Vec2(Float _x, Float _y)
        : x{ _x }
        , y{ _y }
    {
    }

    void SetZero()
    {
        x = Float(0.0);
        y = Float(0.0);
    }

    void Set(Float _x, Float _y)
    {
        x = _x;
        y = _y;
    }

    Float operator[](int32 i) const
    {
        return (&x)[i];
    }

    Float& operator[](int32 i)
    {
        return (&x)[i];
    }

    Vec2 operator-() const
    {
        return Vec2(-x, -y);
    }

    void operator+=(Float s)
    {
        x += s;
        y += s;
    }

    void operator+=(const Vec2& v)
    {
        x += v.x;
        y += v.y;
    }

    void operator-=(Float s)
    {
        x -= s;
        y -= s;
    }

    void operator-=(const Vec2& v)
    {
        x -= v.x;
        y -= v.y;
    }

    void operator*=(Float s)
    {
        x *= s;
        y *= s;
    }

    void operator*=(const Vec2& v)
    {
        x *= v.x;
        y *= v.y;
    }

    void operator/=(Float s)
    {
        operator*=(Float(1.0) / s);
    }

    void Negate()
    {
        x = -x;
        y = -y;
    }

    Float Length() const
    {
        return std::sqrt(x * x + y * y);
    }

    Float Length2() const
    {
        return x * x + y * y;
    }

    Float Normalize()
    {
        Float length = Length();
        if (length < epsilon)
        {
            return Float(0.0);
        }

        Float invLength = Float(1.0) / length;
        x *= invLength;
        y *= invLength;

        return length;
    }

    // Get the skew vector such that dot(skew_vec, other) == cross(vec, other)
    // return cross(1, *this);
    Vec2 Skew() const
    {
        return Vec2(-y, x);
    }

    bool IsNullish() const
    {
        return std::isnan(x) || std::isinf(x) || std::isnan(y) || std::isinf(y);
    }
};

struct Vec3
{
    Float x, y, z;

    Vec3() = default;

    explicit constexpr Vec3(Float _v)
        : x{ _v }
        , y{ _v }
        , z{ _v }
    {
    }

    constexpr Vec3(Float _x, Float _y, Float _z)
        : x{ _x }
        , y{ _y }
        , z{ _z }
    {
    }

    Vec3(const Vec2& _v)
        : x{ _v.x }
        , y{ _v.y }
        , z{ Float(0.0) }
    {
    }

    void SetZero()
    {
        x = Float(0.0);
        y = Float(0.0);
        z = Float(0.0);
    }

    void Set(Float _x, Float _y, Float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    Float operator[](int32 i) const
    {
        return (&x)[i];
    }

    Float& operator[](int32 i)
    {
        return (&x)[i];
    }

    Vec3 operator-() const
    {
        return Vec3(-x, -y, -z);
    }

    void operator+=(Float s)
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

    void operator-=(Float s)
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

    void operator*=(Float s)
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

    void operator/=(Float s)
    {
        operator*=(Float(1.0) / s);
    }

    void Negate()
    {
        x = -x;
        y = -y;
        z = -z;
    }

    Float Length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    Float Length2() const
    {
        return x * x + y * y + z * z;
    }

    Float Normalize()
    {
        Float length = Length();
        if (length < epsilon)
        {
            return Float(0.0);
        }

        Float invLength = Float(1.0) / length;
        x *= invLength;
        y *= invLength;
        z *= invLength;

        return length;
    }

    bool IsNullish() const
    {
        return std::isnan(x) || std::isinf(x) || std::isnan(y) || std::isinf(y) || std::isnan(z) || std::isinf(z);
    }
};

struct Vec4
{
    Float x, y, z, w;

    Vec4() = default;

    constexpr Vec4(Float v)
        : x{ v }
        , y{ v }
        , z{ v }
        , w{ v }
    {
    }

    constexpr Vec4(Float _v, Float _w)
        : x{ _v }
        , y{ _v }
        , z{ _v }
        , w{ _w }
    {
    }

    constexpr Vec4(Float _x, Float _y, Float _z, Float _w)
        : x{ _x }
        , y{ _y }
        , z{ _z }
        , w{ _w }
    {
    }

    constexpr Vec4(const Vec3& _v, Float _w)
        : x{ _v.x }
        , z{ _v.z }
        , y{ _v.y }
        , w{ _w }
    {
    }

    void SetZero()
    {
        x = Float(0.0);
        y = Float(0.0);
        z = Float(0.0);
        w = Float(0.0);
    }

    void Set(Float _x, Float _y, Float _z, Float _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    Vec4 operator-() const
    {
        return Vec4(-x, -y, -z, -w);
    }

    void operator+=(Float s)
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

    void operator-=(Float s)
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

    void operator*=(Float s)
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

    void operator/=(Float s)
    {
        operator*=(Float(1.0) / s);
    }

    Float operator[](int32 i) const
    {
        return (&x)[i];
    }

    Float& operator[](int32 i)
    {
        return (&x)[i];
    }

    void Negate()
    {
        x = -x;
        y = -y;
        z = -z;
        w = -w;
    }

    Float Length() const
    {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    Float Length2() const
    {
        return x * x + y * y + z * z + w * w;
    }

    Float Normalize()
    {
        Float length = Length();
        if (length < epsilon)
        {
            return Float(0.0);
        }

        Float invLength = Float(1.0) / length;
        x *= invLength;
        y *= invLength;
        z *= invLength;
        w *= invLength;

        return length;
    }

    bool IsNullish() const
    {
        return std::isnan(x) || std::isinf(x) || std::isnan(y) || std::isinf(y) || std::isnan(z) || std::isinf(z) ||
               std::isnan(w) || std::isinf(w);
    }
};

// Column major matrices

struct Mat2
{
    Vec2 ex, ey;

    Mat2() = default;

    Mat2(Identity)
        : Mat2(Float(1.0))
    {
    }

    explicit Mat2(Float v)
    {
        // clang-format off
        ex.x = v;       ey.x = Float(0.0);
        ex.y = Float(0.0);    ey.y = v;
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
        ex.x = Float(1.0);    ey.x = Float(0.0);
        ex.y = Float(0.0);    ey.y = Float(1.0);
        // clang-format on
    }

    void SetZero()
    {
        // clang-format off
        ex.x = Float(0.0);    ey.x = Float(0.0);
        ex.y = Float(0.0);    ey.y = Float(0.0);
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

        Float det = ex.x * ey.y - ey.x * ex.y;
        if (det != Float(0.0))
        {
            det = Float(1.0) / det;
        }

        t.ex.x = det * ey.y;
        t.ey.x = -det * ey.x;
        t.ex.y = -det * ex.y;
        t.ey.y = det * ex.x;

        return t;
    }

    Float GetDeterminant() const
    {
        return ex.x * ey.y - ey.x * ex.y;
    }
};

struct Mat3
{
    Vec3 ex, ey, ez;

    Mat3() = default;

    Mat3(Identity)
        : Mat3(Float(1.0))
    {
    }

    explicit Mat3(Float v)
    {
        // clang-format off
        ex.x = v;       ey.x = Float(0.0);    ez.x = Float(0.0);
        ex.y = Float(0.0);    ey.y = v;       ez.y = Float(0.0);
        ex.z = Float(0.0);    ey.z = Float(0.0);    ez.z = v;
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
        ex.x = Float(1.0);    ey.x = Float(0.0);    ez.x = Float(0.0);
        ex.y = Float(0.0);    ey.y = Float(1.0);    ez.y = Float(0.0);
        ex.z = Float(0.0);    ey.z = Float(0.0);    ez.z = Float(1.0);
        // clang-format on
    }

    void SetZero()
    {
        // clang-format off
        ex.x = Float(0.0);    ey.x = Float(0.0);    ez.x = Float(0.0);
        ex.y = Float(0.0);    ey.y = Float(0.0);    ez.y = Float(0.0);
        ex.z = Float(0.0);    ey.z = Float(0.0);    ez.z = Float(0.0);
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
    Mat3 Scale(Float x, Float y);
    Mat3 Rotate(Float z);
    Mat3 Translate(Float x, Float y);
    Mat3 Translate(const Vec2& v);
};

struct Mat4
{
    Vec4 ex, ey, ez, ew;

    Mat4() = default;

    Mat4(Identity)
        : Mat4(Float(1.0))
    {
    }

    explicit Mat4(Float _v)
    {
        // clang-format off
        ex.x = _v;           ey.x = Float(0.0);    ez.x = Float(0.0);    ew.x = Float(0.0);
        ex.y = Float(0.0);    ey.y = _v;           ez.y = Float(0.0);    ew.y = Float(0.0);
        ex.z = Float(0.0);    ey.z = Float(0.0);    ez.z = _v;           ew.z = Float(0.0);
        ex.w = Float(0.0);    ey.w = Float(0.0);    ez.w = Float(0.0);    ew.w = _v;
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
        : ex{ r.ex, Float(0.0) }
        , ey{ r.ey, Float(0.0) }
        , ez{ r.ez, Float(0.0) }
        , ew{ p, Float(1.0) }
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
        ex.x = Float(1.0);    ey.x = Float(0.0);    ez.x = Float(0.0);    ew.x = Float(0.0);
        ex.y = Float(0.0);    ey.y = Float(1.0);    ez.y = Float(0.0);    ew.y = Float(0.0);
        ex.z = Float(0.0);    ey.z = Float(0.0);    ez.z = Float(1.0);    ew.z = Float(0.0);
        ex.w = Float(0.0);    ey.w = Float(0.0);    ez.w = Float(0.0);    ew.w = Float(1.0);
        // clang-format on
    }

    void SetZero()
    {
        // clang-format off
        ex.x = Float(0.0);    ey.x = Float(0.0);    ez.x = Float(0.0);    ew.x = Float(0.0);
        ex.y = Float(0.0);    ey.y = Float(0.0);    ez.y = Float(0.0);    ew.y = Float(0.0);
        ex.z = Float(0.0);    ey.z = Float(0.0);    ez.z = Float(0.0);    ew.z = Float(0.0);
        ex.w = Float(0.0);    ey.w = Float(0.0);    ez.w = Float(0.0);    ew.w = Float(0.0);
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
    Mat4 Scale(Float x, Float y, Float z);
    Mat4 Rotate(Float x, Float y, Float z);
    Mat4 Translate(Float x, Float y, Float z);
    Mat4 Translate(const Vec3& v);
};

struct Quat
{
    Quat() = default;

    Quat(Identity)
        : Quat(Float(1.0))
    {
    }

    Quat(Float _x, Float _y, Float _z, Float _w)
        : x{ _x }
        , y{ _y }
        , z{ _z }
        , w{ _w }
    {
    }

    explicit Quat(Float _w)
        : x{ Float(0.0) }
        , y{ Float(0.0) }
        , z{ Float(0.0) }
        , w{ _w }
    {
    }

    Quat(const Mat3& m);

    Quat(const Vec3& front, const Vec3& up);

    // Axis must be normalized
    Quat(Float angle, const Vec3& unitAxis)
    {
        Float halfAngle = angle * Float(0.5);

        Float s = std::sin(halfAngle);
        x = unitAxis.x * s;
        y = unitAxis.y * s;
        z = unitAxis.z * s;
        w = std::cos(halfAngle);
    }

    Quat operator-()
    {
        return Quat(-x, -y, -z, -w);
    }

    Quat operator*(Float s) const
    {
        return Quat(x * s, y * s, z * s, w * s);
    }

    bool IsIdentity() const
    {
        return x == Float(0.0) && y == Float(0.0) && z == Float(0.0) && w == Float(1.0);
    }

    // Magnitude
    Float Length() const
    {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    Float Length2() const
    {
        return x * x + y * y + z * z + w * w;
    }

    Float Normalize()
    {
        Float length = Length();
        if (length < epsilon)
        {
            return Float(0.0);
        }

        Float invLength = Float(1.0) / length;
        x *= invLength;
        y *= invLength;
        z *= invLength;
        w *= invLength;

        return length;
    }

    Quat GetConjugate() const
    {
        return Quat(-x, -y, -z, w);
    }

    Vec3 GetImaginaryPart() const
    {
        return Vec3(x, y, z);
    }

    // Optimized qvq'
    Vec3 Rotate(const Vec3& v) const
    {
        Float vx = Float(2.0) * v.x;
        Float vy = Float(2.0) * v.y;
        Float vz = Float(2.0) * v.z;
        Float w2 = w * w - Float(0.5);

        Float dot2 = (x * vx + y * vy + z * vz);

        return Vec3((vx * w2 + (y * vz - z * vy) * w + x * dot2), (vy * w2 + (z * vx - x * vz) * w + y * dot2),
                    (vz * w2 + (x * vy - y * vx) * w + z * dot2));
    }

    Vec3 RotateInv(const Vec3& v) const
    {
        Float vx = Float(2.0) * v.x;
        Float vy = Float(2.0) * v.y;
        Float vz = Float(2.0) * v.z;
        Float w2 = w * w - Float(0.5);

        Float dot2 = (x * vx + y * vy + z * vz);

        return Vec3((vx * w2 - (y * vz - z * vy) * w + x * dot2), (vy * w2 - (z * vx - x * vz) * w + y * dot2),
                    (vz * w2 - (x * vy - y * vx) * w + z * dot2));
    }

    void SetIdentity()
    {
        x = Float(0.0);
        y = Float(0.0);
        z = Float(0.0);
        w = Float(1.0);
    }

    // Computes rotation of x-axis
    Vec3 GetBasisX() const
    {
        Float x2 = x * Float(2.0);
        Float w2 = w * Float(2.0);

        return Vec3((w * w2) - Float(1.0) + x * x2, (z * w2) + y * x2, (-y * w2) + z * x2);
    }

    // Computes rotation of y-axis
    Vec3 GetBasisY() const
    {
        Float y2 = y * Float(2.0);
        Float w2 = w * Float(2.0);

        return Vec3((-z * w2) + x * y2, (w * w2) - Float(1.0) + y * y2, (x * w2) + z * y2);
    }

    // Computes rotation of z-axis
    Vec3 GetBasisZ() const
    {
        Float z2 = z * Float(2.0);
        Float w2 = w * Float(2.0);

        return Vec3((y * w2) + x * z2, (-x * w2) + y * z2, (w * w2) - Float(1.0) + z * z2);
    }

    Float x, y, z, w;
};

inline Float Dot(const Vec2& a, const Vec2& b)
{
    return a.x * b.x + a.y * b.y;
}

inline Float Cross(const Vec2& a, const Vec2& b)
{
    return a.x * b.y - a.y * b.x;
}

inline Vec2 Cross(Float s, const Vec2& v)
{
    return Vec2(-s * v.y, s * v.x);
}

inline Vec2 Cross(const Vec2& v, Float s)
{
    return Vec2(s * v.y, -s * v.x);
}

inline Vec2 operator+(const Vec2& a, const Vec2& b)
{
    return Vec2(a.x + b.x, a.y + b.y);
}

inline Vec2 operator-(const Vec2& a, const Vec2& b)
{
    return Vec2(a.x - b.x, a.y - b.y);
}

inline Vec2 operator*(const Vec2& v, Float s)
{
    return Vec2(v.x * s, v.y * s);
}

inline Vec2 operator*(Float s, const Vec2& v)
{
    return operator*(v, s);
}

inline Vec2 operator*(const Vec2& a, const Vec2& b)
{
    return Vec2(a.x * b.x, a.y * b.y);
}

inline Vec2 operator/(const Vec2& v, Float s)
{
    return v * (Float(1.0) / s);
}

inline bool operator==(const Vec2& a, const Vec2& b)
{
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const Vec2& a, const Vec2& b)
{
    return a.x != b.x || a.y != b.y;
}

inline Float Dist(const Vec2& a, const Vec2& b)
{
    return (a - b).Length();
}

inline Float Dist2(const Vec2& a, const Vec2& b)
{
    return (a - b).Length2();
}

inline Float Length(const Vec2& v)
{
    return v.Length();
}

inline Float Length2(const Vec2& v)
{
    return v.Length2();
}

// Vec2 functions end

// Vec3 functions begin

inline Float Dot(const Vec3& a, const Vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 Cross(const Vec3& a, const Vec3& b)
{
    return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline Vec3 operator+(const Vec3& a, const Vec3& b)
{
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vec3 operator-(const Vec3& a, const Vec3& b)
{
    return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vec3 operator*(const Vec3& v, Float s)
{
    return Vec3(v.x * s, v.y * s, v.z * s);
}

inline Vec3 operator*(Float s, const Vec3& v)
{
    return operator*(v, s);
}

inline Vec3 operator*(const Vec3& a, const Vec3& b)
{
    return Vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

inline Vec3 operator/(const Vec3& v, Float s)
{
    return v * (Float(1.0) / s);
}

inline Vec3 operator/(Float s, const Vec3& v)
{
    return (Float(1.0) / s) * v;
}

inline Vec3 operator/(const Vec3& a, const Vec3& b)
{
    return Vec3(a.x / b.x, a.y / b.y, a.z / b.z);
}

inline bool operator==(const Vec3& a, const Vec3& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline bool operator!=(const Vec3& a, const Vec3& b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z;
}

inline Float Dist(const Vec3& a, const Vec3& b)
{
    return (b - a).Length();
}

inline Float Dist2(const Vec3& a, const Vec3& b)
{
    return (b - a).Length2();
}

// Vec3 functions end

// Vec4 functions begin

inline Float Dot(const Vec4& a, const Vec4& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline Vec4 operator+(const Vec4& a, const Vec4& b)
{
    return Vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

inline Vec4 operator-(const Vec4& a, const Vec4& b)
{
    return Vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

inline Vec4 operator*(const Vec4& v, Float s)
{
    return Vec4(v.x * s, v.y * s, v.z * s, v.w * s);
}

inline Vec4 operator*(Float s, const Vec4& v)
{
    return operator*(v, s);
}

inline Vec4 operator*(const Vec4& a, const Vec4& b)
{
    return Vec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

inline Vec4 operator/(const Vec4& v, Float s)
{
    return v * (Float(1.0) / s);
}

inline Vec4 operator/(Float s, const Vec4& v)
{
    return (Float(1.0) / s) * v;
}

inline Vec4 operator/(const Vec4& a, const Vec4& b)
{
    return Vec4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
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

template <typename T>
inline T Normalize(const T& v)
{
    Float invLength = Float(1.0) / v.Length();
    return v * invLength;
}

// Quat functions begin

inline bool operator==(const Quat& a, const Quat& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

inline Float Dot(const Quat& a, const Quat& b)
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
inline Float Angle(const Quat& a, const Quat& b)
{
    return std::acos(Dot(a, b)) * Float(2.0);
}

// Quat functions end

// Mat2 functions begin

inline Mat2 operator+(const Mat2& a, const Mat2& b)
{
    return Mat2(a.ex + b.ex, a.ey + b.ey);
}

// M * V
inline Vec2 Mul(const Mat2& m, const Vec2& v)
{
    return Vec2(m.ex.x * v.x + m.ey.x * v.y, m.ex.y * v.x + m.ey.y * v.y);
}

// M^T * V
inline Vec2 MulT(const Mat2& m, const Vec2& v)
{
    return Vec2(Dot(m.ex, v), Dot(m.ey, v));
}

// A * B
inline Mat2 Mul(const Mat2& a, const Mat2& b)
{
    return Mat2(Mul(a, b.ex), Mul(a, b.ey));
}

// A^T * B
inline Mat2 MulT(const Mat2& a, const Mat2& b)
{
    Vec2 c1(Dot(a.ex, b.ex), Dot(a.ey, b.ex));
    Vec2 c2(Dot(a.ex, b.ey), Dot(a.ey, b.ey));

    return Mat2(c1, c2);
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

// M^T * V
inline Vec3 MulT(const Mat3& m, const Vec3& v)
{
    return Vec3(Dot(m.ex, v), Dot(m.ey, v), Dot(m.ez, v));
}

// A * B
inline Mat3 Mul(const Mat3& a, const Mat3& b)
{
    return Mat3(Mul(a, b.ex), Mul(a, b.ey), Mul(a, b.ez));
}

// A^T * B
inline Mat3 MulT(const Mat3& a, const Mat3& b)
{
    Vec3 c1(Dot(a.ex, b.ex), Dot(a.ey, b.ex), Dot(a.ez, b.ex));
    Vec3 c2(Dot(a.ex, b.ey), Dot(a.ey, b.ey), Dot(a.ez, b.ey));
    Vec3 c3(Dot(a.ex, b.ez), Dot(a.ey, b.ez), Dot(a.ez, b.ez));

    return Mat3(c1, c2, c3);
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

// M^T * V
inline Vec4 MulT(const Mat4& m, const Vec4& v)
{
    return Vec4(Dot(m.ex, v), Dot(m.ey, v), Dot(m.ez, v), Dot(m.ew, v));
}

// A * B
inline Mat4 Mul(const Mat4& a, const Mat4& b)
{
    return Mat4(Mul(a, b.ex), Mul(a, b.ey), Mul(a, b.ez), Mul(a, b.ew));
}

// A^T * B
inline Mat4 MulT(const Mat4& a, const Mat4& b)
{
    Vec4 c1(Dot(a.ex, b.ex), Dot(a.ey, b.ex), Dot(a.ez, b.ex), Dot(a.ew, b.ex));
    Vec4 c2(Dot(a.ex, b.ey), Dot(a.ey, b.ey), Dot(a.ez, b.ey), Dot(a.ew, b.ey));
    Vec4 c3(Dot(a.ex, b.ez), Dot(a.ey, b.ez), Dot(a.ez, b.ez), Dot(a.ew, b.ez));
    Vec4 c4(Dot(a.ex, b.ew), Dot(a.ey, b.ew), Dot(a.ez, b.ew), Dot(a.ew, b.ew));

    return Mat4(c1, c2, c3, c4);
}

inline Mat4 Orth(Float left, Float right, Float bottom, Float top, Float zNear, Float zFar)
{
    Mat4 t{ Float(1.0) };

    // Scale
    t.ex.x = Float(2.0) / (right - left);
    t.ey.y = Float(2.0) / (top - bottom);
    t.ez.z = Float(2.0) / (zFar - zNear);

    // Translation
    t.ew.x = -(right + left) / (right - left);
    t.ew.y = -(top + bottom) / (top - bottom);
    t.ew.z = -(zFar + zNear) / (zFar - zNear);

    return t;
}

// Mat4 functions end

} // namespace spt