#pragma once

#include "math.h"

namespace spt
{

constexpr Vec3 zero_vec3{ 0.0, 0.0, 0.0 };
constexpr Vec3 x_axis{ 1.0, 0.0, 0.0 };
constexpr Vec3 y_axis{ 0.0, 1.0, 0.0 };
constexpr Vec3 z_axis{ 0.0, 0.0, 1.0 };

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

inline thread_local std::minstd_rand prng;

inline void Srand(uint32 seed)
{
    prng.seed(seed);
}

inline Real Prand()
{
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

inline Vec3 RandVec3()
{
    return Vec3{ Rand(), Rand(), Rand() };
}

inline static Vec3 RandVec3(Real min, Real max)
{
    return Vec3{ Rand(min, max), Rand(min, max), Rand(min, max) };
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

inline Vec3 PolarToCart(Real theta, Real phi, Real r = Real(1.0))
{
    Real sin_thetha = sin(theta);
    Real x = cos(phi) * sin_thetha;
    Real y = sin(phi) * sin_thetha;
    Real z = cos(theta);

    return Vec3{ x * r, y * r, z * r };
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
    Real z = sqrt(1.0 - r2);

    Real phi = 2.0 * pi * r1;
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

inline Vec3 RandomInUnitDiskXY()
{
    return Vec3{ Rand(-1.0, 1.0), Rand(-1.0, 1.0), 0.0 };
}

} // namespace spt
