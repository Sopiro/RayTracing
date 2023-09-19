#pragma once

#include "math.h"
#include "random.h"

namespace spt
{

constexpr Vec2 zero_vec2{ Real(0.0), Real(0.0) };
constexpr Vec3 zero_vec3{ Real(0.0), Real(0.0), Real(0.0) };
constexpr Vec4 zero_vec4{ Real(0.0), Real(0.0), Real(0.0), Real(0.0) };

constexpr Vec3 x_axis{ Real(1.0), Real(0.0), Real(0.0) };
constexpr Vec3 y_axis{ Real(0.0), Real(1.0), Real(0.0) };
constexpr Vec3 z_axis{ Real(0.0), Real(0.0), Real(1.0) };

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

inline Real DegToRad(Real deg)
{
    return Real(deg * pi / Real(180.0));
}

inline Real RadToDeg(Real rad)
{
    return Real(rad * inv_pi * Real(180.0));
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

template <typename T>
inline T Clamp(T v, T _min, T _max)
{
    return Max(_min, Min(v, _max));
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
    Real angle = std::acos(dot) * percent;

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
    return v - 2 * Dot(v, n) * n;
}

template <typename T>
T Refract(const T& uv, const T& n, Real etai_over_etat)
{
    Real cos_theta = std::fmin(Dot(-uv, n), Real(1.0));
    T r_out_perp = etai_over_etat * (uv + cos_theta * n);
    T r_out_parallel = -std::sqrt(std::fabs(Real(1.0) - r_out_perp.Length2())) * n;

    return r_out_perp + r_out_parallel;
}

inline Vec3 PolarToCart(Real theta, Real phi, Real r = Real(1.0))
{
    Real sin_thetha = std::sin(theta);
    Real x = std::cos(phi) * sin_thetha;
    Real y = std::sin(phi) * sin_thetha;
    Real z = std::cos(theta);

    return Vec3{ x * r, y * r, z * r };
}

inline UV ComputeSphereUV(const Vec3& dir)
{
    f64 theta = std::acos(-dir.y);
    f64 phi = std::atan2(-dir.z, dir.x) + pi;

    return UV{ phi * inv_two_pi, theta * inv_pi };
}

} // namespace spt
