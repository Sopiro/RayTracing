#pragma once

#include "math.h"
#include "random.h"

namespace bulbit
{

constexpr Vec3 x_axis{ 1, 0, 0 };
constexpr Vec3 y_axis{ 0, 1, 0 };
constexpr Vec3 z_axis{ 0, 0, 1 };

template <typename T>
constexpr inline T Sqr(T v)
{
    return v * v;
}

constexpr inline Float DegToRad(Float deg)
{
    return Float(deg * pi / 180);
}

constexpr inline Float RadToDeg(Float rad)
{
    return Float(rad * inv_pi * 180);
}

template <typename T>
constexpr inline T Abs(T a)
{
    return a > T(0) ? a : -a;
}

template <typename T>
constexpr inline Float AbsDot(T a, T b)
{
    return std::abs(Dot(a, b));
}

template <typename T>
constexpr inline T Min(T a, T b)
{
    return a < b ? a : b;
}

template <typename T>
constexpr inline T Max(T a, T b)
{
    return a > b ? a : b;
}

constexpr inline Vec2 Min(const Vec2& a, const Vec2& b)
{
    return Vec2(std::fmin(a.x, b.x), std::fmin(a.y, b.y));
}

constexpr inline Vec2 Max(const Vec2& a, const Vec2& b)
{
    return Vec2(std::fmax(a.x, b.x), std::fmax(a.y, b.y));
}

constexpr inline Vec3 Min(const Vec3& a, const Vec3& b)
{
    return Vec3(std::fmin(a.x, b.x), std::fmin(a.y, b.y), std::fmin(a.z, b.z));
}

constexpr inline Vec3 Max(const Vec3& a, const Vec3& b)
{
    return Vec3(std::fmax(a.x, b.x), std::fmax(a.y, b.y), std::fmax(a.z, b.z));
}

template <typename T, typename U, typename V>
constexpr inline T Clamp(T v, U l, V r)
{
    if (v < l)
    {
        return T(l);
    }
    else if (v > r)
    {
        return T(r);
    }
    else
    {
        return v;
    }
}

template <typename T>
constexpr inline T Lerp(const T& a, const T& b, Float t)
{
    return a * (1 - t) + b * t;
}

template <typename T>
constexpr inline T Slerp(const T& start, const T& end, Float percent)
{
    Float dot = Clamp(Dot(start, end), -Float(1.0), Float(1.0));
    Float angle = std::acos(dot) * percent;

    T rv = end - start * dot;
    rv.Normalize();

    return start * std::cos(angle) + rv * std::sin(angle);
}

template <typename T>
constexpr inline T Project(const T& v, const T& n)
{
    return v - n * Dot(v, n);
}

template <typename T>
constexpr inline T Reflect(const T& v, const T& n)
{
    return -v + 2 * Dot(v, n) * n;
}

template <typename Predicate>
constexpr inline int32 FindInterval(int32 size, const Predicate& pred)
{
    int32 first = 0, len = size;
    while (len > 0)
    {
        int32 half = len >> 1, middle = first + half;
        if (pred(middle))
        {
            first = middle + 1;
            len -= half + 1;
        }
        else
        {
            len = half;
        }
    }
    return Clamp(first - 1, 0, size - 2);
}

} // namespace bulbit
