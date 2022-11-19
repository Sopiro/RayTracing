// Simple linear math library
// Highly inspired by Box2d, glm math codes

#pragma once

#include <float.h>
#include <limits>
#include <math.h>
#include <random>
#include <stdint.h>

#include "types.h"

#define precision double

constexpr precision pi = precision(3.14159265358979323846);
constexpr precision piDiv2 = precision(3.14159265358979323846 / 2.0);
constexpr precision infinity = std::numeric_limits<precision>::infinity();
constexpr precision epsilon = std::numeric_limits<precision>::epsilon();

enum Identity
{
    identity
};

inline precision Prand()
{
    static std::minstd_rand prng;
    return precision(prng()) / std::minstd_rand::max();
}

inline precision Prand(precision min, precision max)
{
    return min + (max - min) * Prand();
}

inline precision DegToRad(precision degrees)
{
    return precision(degrees * pi / 180.0);
}

inline precision Rand()
{
    static std::uniform_real_distribution<precision> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline precision Rand(precision min, precision max)
{
    return min + (max - min) * Rand();
}

struct Vec2
{
    precision x, y;

    Vec2() = default;

    constexpr Vec2(precision _v)
        : x{ _v }
        , y{ _v }
    {
    }

    constexpr Vec2(precision _x, precision _y)
        : x{ _x }
        , y{ _y }
    {
    }

    void SetZero()
    {
        x = precision(0.0);
        y = precision(0.0);
    }

    void Set(precision _x, precision _y)
    {
        x = _x;
        y = _y;
    }

    precision operator[](uint32 i) const
    {
        return (&x)[i];
    }

    precision& operator[](uint32 i)
    {
        return (&x)[i];
    }

    Vec2 operator-() const
    {
        return Vec2{ -x, -y };
    }

    void operator+=(const Vec2& v)
    {
        x += v.x;
        y += v.y;
    }

    void operator-=(const Vec2& v)
    {
        x -= v.x;
        y -= v.y;
    }

    void operator+=(precision s)
    {
        x += s;
        y += s;
    }

    void operator-=(precision s)
    {
        x -= s;
        y -= s;
    }

    void operator*=(precision s)
    {
        x *= s;
        y *= s;
    }

    void operator/=(precision s)
    {
        operator*=(1.0 / s);
    }

    precision Length() const
    {
        return sqrt(x * x + y * y);
    }

    precision Length2() const
    {
        return x * x + y * y;
    }

    // Optimized to not check length == 0
    precision Normalize()
    {
        precision length = Length();
        precision invLength = precision(1.0) / length;
        x *= invLength;
        y *= invLength;

        return length;
    }

    // Optimized to not check length == 0
    Vec2 Normalized() const
    {
        precision invLength = precision(1.0) / Length();

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
    precision x, y, z;

    Vec3() = default;

    inline static Vec3 Random()
    {
        return Vec3{ Rand(), Rand(), Rand() };
    }

    inline static Vec3 Random(precision min, precision max)
    {
        return Vec3{ Rand(min, max), Rand(min, max), Rand(min, max) };
    }

    constexpr Vec3(precision _v)
        : x{ _v }
        , y{ _v }
        , z{ _v }
    {
    }

    constexpr Vec3(precision _x, precision _y, precision _z)
        : x{ _x }
        , y{ _y }
        , z{ _z }
    {
    }

    Vec3(const Vec2& _v)
        : x{ _v.x }
        , y{ _v.y }
        , z{ 0.0f }
    {
    }

    void SetZero()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    void Set(precision _x, precision _y, precision _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    precision operator[](uint32 i) const
    {
        return (&x)[i];
    }

    precision& operator[](uint32 i)
    {
        return (&x)[i];
    }

    Vec3 operator-() const
    {
        return Vec3{ -x, -y, -z };
    }

    void operator+=(const Vec3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    void operator-=(const Vec3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    void operator+=(precision s)
    {
        x += s;
        y += s;
        z += s;
    }

    void operator-=(precision s)
    {
        x -= s;
        y -= s;
        z -= s;
    }

    void operator*=(precision s)
    {
        x *= s;
        y *= s;
        z *= s;
    }

    void operator/=(precision s)
    {
        operator*=(precision(1.0) / s);
    }

    precision Length() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    precision Length2() const
    {
        return x * x + y * y + z * z;
    }

    precision Normalize()
    {
        precision length = Length();
        precision invLength = precision(1.0) / length;
        x *= invLength;
        y *= invLength;
        z *= invLength;

        return length;
    }

    Vec3 Normalized() const
    {
        precision length = Length();
        if (length < epsilon)
        {
            return Vec3{ precision(0.0), precision(0.0), precision(0.0) };
        }

        precision invLength = precision(1.0) / length;

        return Vec3{ x * invLength, y * invLength, z * invLength };
    }
};

struct Vec4
{
    precision x, y, z, w;

    Vec4() = default;

    constexpr Vec4(precision _v, precision _w)
        : x{ _v }
        , y{ _v }
        , z{ _v }
        , w{ _w }
    {
    }

    constexpr Vec4(precision _x, precision _y, precision _z, precision _w)
        : x{ _x }
        , y{ _y }
        , z{ _z }
        , w{ _w }
    {
    }

    void SetZero()
    {
        x = precision(0.0);
        y = precision(0.0);
        z = precision(0.0);
        w = precision(0.0);
    }

    void Set(precision _x, precision _y, precision _z, precision _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    precision operator[](uint32 i) const
    {
        return (&x)[i];
    }

    precision& operator[](uint32 i)
    {
        return (&x)[i];
    }

    precision Length() const
    {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    precision Length2() const
    {
        return x * x + y * y + z * z + w * w;
    }

    precision Normalize()
    {
        precision length = Length();
        precision invLength = precision(1.0) / length;
        x *= invLength;
        y *= invLength;
        z *= invLength;
        w *= invLength;

        return length;
    }
};

struct Quat;

// Column major matrices

struct Mat2
{
    Vec2 ex, ey;

    Mat2() = default;

    Mat2(Identity)
        : Mat2(precision(1.0))
    {
    }

    Mat2(precision v)
    {
        // clang-format off
        ex.x = v;       ey.x = 0.0f;
        ex.y = 0.0f;    ey.y = v;
        // clang-format on
    }

    constexpr Mat2(const Vec2& c1, const Vec2& c2)
        : ex{ c1 }
        , ey{ c2 }
    {
    }

    Vec2& operator[](uint32 i)
    {
        return (&ex)[i];
    }

    void SetIdentity()
    {
        // clang-format off
        ex.x = 1.0f;    ey.x = 0.0f;
        ex.y = 0.0f;    ey.y = 1.0f;
        // clang-format on
    }

    void SetZero()
    {
        // clang-format off
        ex.x = 0.0f;    ey.x = 0.0f;
        ex.y = 0.0f;    ey.y = 0.0f;
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

        precision det = ex.x * ey.y - ey.x * ex.y;
        if (det != 0.0f)
        {
            det = 1.0f / det;
        }

        t.ex.x = det * ey.y;
        t.ey.x = -det * ey.x;
        t.ex.y = -det * ex.y;
        t.ey.y = det * ex.x;

        return t;
    }

    precision GetDeterminant() const
    {
        return ex.x * ey.y - ey.x * ex.y;
    }
};

struct Mat3
{
    Vec3 ex, ey, ez;

    Mat3() = default;

    Mat3(Identity)
        : Mat3(precision(1.0))
    {
    }

    Mat3(precision v)
    {
        // clang-format off
        ex.x = v;       ey.x = 0.0f;    ez.x = 0.0f;
        ex.y = 0.0f;    ey.y = v;       ez.y = 0.0f;
        ex.z = 0.0f;    ey.z = 0.0f;    ez.z = v;
        // clang-format on
    }

    constexpr Mat3(const Vec3& c1, const Vec3& c2, const Vec3& c3)
        : ex{ c1 }
        , ey{ c2 }
        , ez{ c3 }
    {
    }

    Mat3(const Quat& q);

    Vec3& operator[](uint32 i)
    {
        return (&ex)[i];
    }

    void SetIdentity()
    {
        // clang-format off
        ex.x = 1.0f;    ey.x = 0.0f;    ez.x = 0.0f;
        ex.y = 0.0f;    ey.y = 1.0f;    ez.y = 0.0f;
        ex.z = 0.0f;    ey.z = 0.0f;    ez.z = 1.0f;
        // clang-format on
    }

    void SetZero()
    {
        // clang-format off
        ex.x = 0.0f;    ey.x = 0.0f;    ez.x = 0.0f;
        ex.y = 0.0f;    ey.y = 0.0f;    ez.y = 0.0f;
        ex.z = 0.0f;    ey.z = 0.0f;    ez.z = 0.0f;
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
    Mat3 Scale(precision x, precision y);
    Mat3 Rotate(precision z);
    Mat3 Translate(precision x, precision y);
    Mat3 Translate(const Vec2& v);
};

struct Mat4
{
    Vec4 ex, ey, ez, ew;

    Mat4() = default;

    Mat4(Identity)
        : Mat4(precision(1.0))
    {
    }

    Mat4(precision _v)
    {
        // clang-format off
        ex.x = _v;      ey.x = 0.0f;    ez.x = 0.0f;    ew.x = 0.0f;
        ex.y = 0.0f;    ey.y = _v;      ez.y = 0.0f;    ew.y = 0.0f;
        ex.z = 0.0f;    ey.z = 0.0f;    ez.z = _v;      ew.z = 0.0f;
        ex.w = 0.0f;    ey.w = 0.0f;    ez.w = 0.0f;    ew.w = _v;
        // clang-format on
    }

    constexpr Mat4(const Vec4& c1, const Vec4& c2, const Vec4& c3, const Vec4& c4)
        : ex{ c1 }
        , ey{ c2 }
        , ez{ c3 }
        , ew{ c4 }
    {
    }

    Vec4& operator[](uint32 i)
    {
        return (&ex)[i];
    }

    void SetIdentity()
    {
        // clang-format off
        ex.x = 1.0f;    ey.x = 0.0f;    ez.x = 0.0f;    ew.x = 0.0f;
        ex.y = 0.0f;    ey.y = 1.0f;    ez.y = 0.0f;    ew.y = 0.0f;
        ex.z = 0.0f;    ey.z = 0.0f;    ez.z = 1.0f;    ew.z = 0.0f;
        ex.w = 0.0f;    ey.w = 0.0f;    ez.w = 0.0f;    ew.w = 1.0f;
        // clang-format on
    }

    void SetZero()
    {
        // clang-format off
        ex.x = 0.0f;    ey.x = 0.0f;    ez.x = 0.0f;    ew.x = 0.0f;
        ex.y = 0.0f;    ey.y = 0.0f;    ez.y = 0.0f;    ew.y = 0.0f;
        ex.z = 0.0f;    ey.z = 0.0f;    ez.z = 0.0f;    ew.z = 0.0f;
        ex.w = 0.0f;    ey.w = 0.0f;    ez.w = 0.0f;    ew.w = 0.0f;
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
    Mat4 Scale(precision x, precision y, precision z);
    Mat4 Rotate(precision x, precision y, precision z);
    Mat4 Translate(precision x, precision y, precision z);
    Mat4 Translate(const Vec3& v);
};

struct Quat
{
    Quat() = default;

    Quat(Identity)
        : Quat(precision(1.0))
    {
    }

    Quat(precision _x, precision _y, precision _z, precision _w)
        : x{ _x }
        , y{ _y }
        , z{ _z }
        , w{ _w }
    {
    }

    Quat(precision _w)
        : x{ precision(0.0) }
        , y{ precision(0.0) }
        , z{ precision(0.0) }
        , w{ _w }
    {
    }

    Quat(const Mat3& m);

    Quat(const Vec3& dir, const Vec3& up);

    // Axis must be normalized
    Quat(precision angle, const Vec3& unitAxis)
    {
        precision halfAngle = angle * precision(0.5);

        precision s = sin(halfAngle);
        x = unitAxis.x * s;
        y = unitAxis.y * s;
        z = unitAxis.z * s;
        w = cos(halfAngle);
    }

    Quat operator-()
    {
        return Quat{ -x, -y, -z, -w };
    }

    Quat operator*(precision s) const
    {
        return Quat{ x * s, y * s, z * s, w * s };
    }

    bool IsIdentity() const
    {
        return x == precision(0.0) && y == precision(0.0) && z == precision(0.0) && w == precision(1.0);
    }

    // Magnitude
    precision Length() const
    {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    precision Length2() const
    {
        return x * x + y * y + z * z + w * w;
    }

    precision Normalize()
    {
        precision length = Length();
        precision invLength = precision(1.0) / length;
        x *= invLength;
        y *= invLength;
        z *= invLength;
        w *= invLength;

        return length;
    }

    Quat Normalized() const
    {
        precision length = Length();
        if (length < epsilon)
        {
            return Quat{ precision(0.0), precision(0.0), precision(0.0), precision(0.0) };
        }

        precision invLength = precision(1.0) / length;

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
        precision vx = precision(2.0) * v.x;
        precision vy = precision(2.0) * v.y;
        precision vz = precision(2.0) * v.z;
        precision w2 = w * w - precision(0.5);

        precision dot2 = (x * vx + y * vy + z * vz);

        return Vec3((vx * w2 + (y * vz - z * vy) * w + x * dot2), (vy * w2 + (z * vx - x * vz) * w + y * dot2),
                    (vz * w2 + (x * vy - y * vx) * w + z * dot2));
    }

    Vec3 RotateInv(const Vec3& v) const
    {
        precision vx = precision(2.0) * v.x;
        precision vy = precision(2.0) * v.y;
        precision vz = precision(2.0) * v.z;
        precision w2 = w * w - precision(0.5);

        precision dot2 = (x * vx + y * vy + z * vz);

        return Vec3((vx * w2 - (y * vz - z * vy) * w + x * dot2), (vy * w2 - (z * vx - x * vz) * w + y * dot2),
                    (vz * w2 - (x * vy - y * vx) * w + z * dot2));
    }

    void SetIdentity()
    {
        x = precision(0.0);
        y = precision(0.0);
        z = precision(0.0);
        w = precision(1.0);
    }

    precision x, y, z, w;
};

inline precision Dot(const Vec2& a, const Vec2& b)
{
    return a.x * b.x + a.y * b.y;
}

inline precision Cross(const Vec2& a, const Vec2& b)
{
    return a.x * b.y - a.y * b.x;
}

inline Vec2 Cross(precision s, const Vec2& v)
{
    return Vec2{ -s * v.y, s * v.x };
}

inline Vec2 Cross(const Vec2& v, precision s)
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

inline Vec2 operator*(const Vec2& v, precision s)
{
    return Vec2{ v.x * s, v.y * s };
}

inline Vec2 operator*(precision s, const Vec2& v)
{
    return operator*(v, s);
}

inline Vec2 operator*(const Vec2& a, const Vec2& b)
{
    return Vec2{ a.x * b.x, a.y * b.y };
}

inline Vec2 operator/(const Vec2& v, precision s)
{
    return v * (1.0f / s);
}

inline bool operator==(const Vec2& a, const Vec2& b)
{
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const Vec2& a, const Vec2& b)
{
    return a.x != b.x || a.y != b.y;
}

inline precision Dist(const Vec2& a, const Vec2& b)
{
    return (a - b).Length();
}

inline precision Dist2(const Vec2& a, const Vec2& b)
{
    return (a - b).Length2();
}

inline precision Length(const Vec2& v)
{
    return v.Length();
}

inline precision Length2(const Vec2& v)
{
    return v.Length2();
}

inline Vec2 Normalize(const Vec2& v)
{
    return v.Normalized();
}

// Vec2 functions end

// Vec3 functions begin

inline precision Dot(const Vec3& a, const Vec3& b)
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

inline Vec3 operator*(const Vec3& v, precision s)
{
    return Vec3{ v.x * s, v.y * s, v.z * s };
}

inline Vec3 operator*(precision s, const Vec3& v)
{
    return operator*(v, s);
}

inline Vec3 operator*(const Vec3& a, const Vec3& b)
{
    return Vec3{ a.x * b.x, a.y * b.y, a.z * b.z };
}

inline Vec3 operator/(const Vec3& v, precision s)
{
    return v * (1.0f / s);
}

inline bool operator==(const Vec3& a, const Vec3& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline bool operator!=(const Vec3& a, const Vec3& b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z;
}

inline precision Dist(const Vec3& a, const Vec3& b)
{
    return (b - a).Length();
}

inline precision Dist2(const Vec3& a, const Vec3& b)
{
    return (b - a).Length2();
}

// Vec3 functions end

// Vec4 functions begin

inline precision Dot(const Vec4& a, const Vec4& b)
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

inline precision Dot(const Quat& a, const Quat& b)
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
inline precision Angle(const Quat& a, const Quat& b)
{
    return acos(Dot(a, b)) * precision(2.0);
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

inline Mat4 Orth(precision left, precision right, precision bottom, precision top, precision zNear, precision zFar)
{
    Mat4 t{ 1.0f };

    // Scale
    t.ex.x = 2.0f / (right - left);
    t.ey.y = 2.0f / (top - bottom);
    t.ez.z = 2.0f / (zFar - zNear);

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
inline T Lerp(const T& a, const T& b, precision t)
{
    return a * (1.0f - t) + b * t;
}

template <typename T>
inline T Slerp(const T& start, const T& end, precision percent)
{
    precision dot = Clamp(Dot(start, end), -1.0f, 1.0f);
    precision angle = acosf(dot) * percent;

    T rv = end - start * dot;
    rv.Normalize();

    return start * Cos(angle) + rv * Sin(angle);
}

template <typename T>
inline T Reflect(const T& v, const T& n)
{
    return v - 2 * Dot(v, n) * n;
}

template <typename T>
T Refract(const T& uv, const T& n, double etai_over_etat)
{
    auto cos_theta = fmin(Dot(-uv, n), 1.0);
    T r_out_perp = etai_over_etat * (uv + cos_theta * n);
    T r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.Length2())) * n;

    return r_out_perp + r_out_parallel;
}

inline Vec3 PolarToCart(precision lat, precision lgt, precision r)
{
    precision x = sin(lat) * cos(lgt);
    precision y = sin(lat) * sin(lgt);
    precision z = cos(lat);

    return Vec3{ x, y, z } * r;
}

inline Vec3 RandomUnitVector()
{
    precision theta = 2 * pi * Rand();
    precision phi = pi * Rand();
    // precision phi = acos(1.0 - 2.0 * Rand());

    precision x = sin(phi) * cos(theta);
    precision y = sin(phi) * sin(theta);
    precision z = cos(phi);

    return Vec3{ x, y, z };
}

inline Vec3 RandomInUnitSphere()
{
    precision r = Rand();

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