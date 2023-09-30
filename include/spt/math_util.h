#pragma once

#include "math.h"
#include "random.h"

namespace spt
{

constexpr Vec2 zero_vec2{ Float(0.0), Float(0.0) };
constexpr Vec3 zero_vec3{ Float(0.0), Float(0.0), Float(0.0) };
constexpr Vec4 zero_vec4{ Float(0.0), Float(0.0), Float(0.0), Float(0.0) };

constexpr Vec3 x_axis{ Float(1.0), Float(0.0), Float(0.0) };
constexpr Vec3 y_axis{ Float(0.0), Float(1.0), Float(0.0) };
constexpr Vec3 z_axis{ Float(0.0), Float(0.0), Float(1.0) };

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline std::ostream& operator<<(std::ostream& out, const Mat4& m)
{
    // clang-format off
    return out << m.ex.x << ' ' << m.ey.x << ' ' << m.ez.x << ' ' << m.ew.x << '\n'
               << m.ex.y << ' ' << m.ey.y << ' ' << m.ez.y << ' ' << m.ew.y << '\n'
               << m.ex.z << ' ' << m.ey.z << ' ' << m.ez.z << ' ' << m.ew.z << '\n'
               << m.ex.w << ' ' << m.ey.w << ' ' << m.ez.w << ' ' << m.ew.w << '\n';

    // clang-format on
}

inline Mat4 Convert(const aiMatrix4x4& aiMat)
{
    Mat4 t;
    t.ex.Set(aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1);
    t.ey.Set(aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2);
    t.ez.Set(aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3);
    t.ew.Set(aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4);

    return t;
}

inline Float DegToRad(Float deg)
{
    return Float(deg * pi / Float(180.0));
}

inline Float RadToDeg(Float rad)
{
    return Float(rad * inv_pi * Float(180.0));
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
T Refract(const T& uv, const T& n, Float etai_over_etat)
{
    Float cos_theta = std::fmin(Dot(-uv, n), Float(1.0));
    T r_out_perp = etai_over_etat * (uv + cos_theta * n);
    T r_out_parallel = -std::sqrt(std::fabs(Float(1.0) - r_out_perp.Length2())) * n;

    return r_out_perp + r_out_parallel;
}

inline Vec3 PolarToCart(Float theta, Float phi, Float r = Float(1.0))
{
    Float sin_thetha = std::sin(theta);
    Float x = std::cos(phi) * sin_thetha;
    Float y = std::sin(phi) * sin_thetha;
    Float z = std::cos(theta);

    return Vec3(x * r, y * r, z * r);
}

inline Point2 ComputeSphereTexCoord(const Vec3& dir)
{
    Float theta = std::acos(-dir.y);
    Float phi = std::atan2(-dir.z, dir.x) + pi;

    return Point2(phi * inv_two_pi, theta * inv_pi);
}

template <typename Predicate>
int32 FindInterval(int32 size, const Predicate& pred)
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

} // namespace spt
