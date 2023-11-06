#pragma once

#include "math.h"
#include "random.h"

namespace bulbit
{

constexpr Vec2 zero_vec2{ 0, 0 };
constexpr Vec3 zero_vec3{ 0, 0, 0 };
constexpr Vec4 zero_vec4{ 0, 0, 0, 0 };

constexpr Vec3 x_axis{ 1, 0, 0 };
constexpr Vec3 y_axis{ 0, 1, 0 };
constexpr Vec3 z_axis{ 0, 0, 1 };

inline Float DegToRad(Float deg)
{
    return Float(deg * pi / 180);
}

inline Float RadToDeg(Float rad)
{
    return Float(rad * inv_pi * 180);
}

template <typename T>
inline T Abs(T a)
{
    return a > T(0) ? a : -a;
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
    return Vec2(std::fmin(a.x, b.x), std::fmin(a.y, b.y));
}

inline Vec2 Max(const Vec2& a, const Vec2& b)
{
    return Vec2(std::fmax(a.x, b.x), std::fmax(a.y, b.y));
}

inline Vec3 Min(const Vec3& a, const Vec3& b)
{
    return Vec3(std::fmin(a.x, b.x), std::fmin(a.y, b.y), std::fmin(a.z, b.z));
}

inline Vec3 Max(const Vec3& a, const Vec3& b)
{
    return Vec3(std::fmax(a.x, b.x), std::fmax(a.y, b.y), std::fmax(a.z, b.z));
}

inline Float Clamp(Float v, Float _min, Float _max)
{
    return std::fmax(_min, std::fmin(v, _max));
}

template <typename T>
inline T Clamp(T v, T _min, T _max)
{
    return Max(_min, Min(v, _max));
}

template <typename T>
inline T Lerp(const T& a, const T& b, Float t)
{
    return a * (Float(1.0) - t) + b * t;
}

template <typename T>
inline T Slerp(const T& start, const T& end, Float percent)
{
    Float dot = Clamp(Dot(start, end), -Float(1.0), Float(1.0));
    Float angle = std::acos(dot) * percent;

    T rv = end - start * dot;
    rv.Normalize();

    return start * std::cos(angle) + rv * std::sin(angle);
}

template <typename T>
inline T Project(const T& v, const T& n)
{
    return v - n * Dot(v, n);
}

template <typename T>
inline T Reflect(const T& v, const T& n)
{
    return -v + 2 * Dot(v, n) * n;
}

template <typename T>
inline T Refract(const T& uv, const T& n, Float etai_over_etat)
{
    Float cos_theta = std::fmin(Dot(-uv, n), Float(1.0));
    T r_out_perp = etai_over_etat * (uv + cos_theta * n);
    T r_out_parallel = -std::sqrt(std::fabs(Float(1.0) - r_out_perp.Length2())) * n;

    return r_out_perp + r_out_parallel;
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

inline Vec3 SphericalDirection(Float sin_theta, Float cos_theta, Float sin_phi, Float cos_phi)
{
    return Vec3(cos_phi * sin_theta, cos_theta, sin_phi * sin_theta);
}

template <typename Predicate>
inline int32 FindInterval(int32 size, const Predicate& pred)
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
