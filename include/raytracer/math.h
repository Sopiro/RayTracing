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

constexpr precision pi = (precision)(3.1415926535897932385);
constexpr precision infinity = std::numeric_limits<precision>::infinity();

inline precision DegToRad(precision degrees)
{
    return (precision)(degrees * pi / 180.0);
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
        x = (precision)0.0;
        y = (precision)0.0;
    }

    void Set(precision _x, precision _y)
    {
        x = _x;
        y = _y;
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
        operator*=((precision)1.0 / s);
    }

    precision Length() const
    {
        return (precision)sqrt(x * x + y * y);
    }

    precision Length2() const
    {
        return x * x + y * y;
    }

    // Optimized to not check length == 0
    precision Normalize()
    {
        precision length = Length();
        precision invLength = (precision)1.0 / length;
        x *= invLength;
        y *= invLength;

        return length;
    }

    // Optimized to not check length == 0
    Vec2 Normalized() const
    {
        precision invLength = (precision)1.0 / Length();

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
        operator*=((precision)1.0 / s);
    }

    precision Length() const
    {
        return (precision)sqrt(x * x + y * y + z * z);
    }

    precision Length2() const
    {
        return x * x + y * y + z * z;
    }

    precision Normalize()
    {
        precision length = Length();
        precision invLength = (precision)1.0 / length;
        x *= invLength;
        y *= invLength;
        z *= invLength;

        return length;
    }

    Vec3 Normalized() const
    {
        precision length = Length();
        if (length < FLT_EPSILON)
        {
            return Vec3{ (precision)0.0, (precision)0.0, (precision)0.0 };
        }

        precision invLength = (precision)1.0 / length;

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

    precision& operator[](uint32 i)
    {
        return (&x)[i];
    }
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