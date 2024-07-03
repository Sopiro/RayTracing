// Simple linear math library
// Highly inspired by Box2d, glm math codes

#pragma once

#include "floats.h"
#include "vector.h"

namespace bulbit
{

enum Identity
{
    identity
};

using Vec2f = Vector2<Float>;
using Vec2i = Vector2<int32>;
using Vec2 = Vec2f;
using Point2 = Vec2f;

using Vec3f = Vector3<Float>;
using Vec3i = Vector3<int32>;
using Vec3 = Vec3f;
using Point3 = Vec3f;

using Vec4f = Vector4<Float>;
using Vec4i = Vector4<int32>;
using Vec4 = Vec4f;

constexpr inline Vec3 x_axis{ 1, 0, 0 };
constexpr inline Vec3 y_axis{ 0, 1, 0 };
constexpr inline Vec3 z_axis{ 0, 0, 1 };

template <template <typename> class V, typename T>
inline V<T> Normalize(const V<T>& v)
{
    T inv_length = T(1) / Length(v);
    return v * inv_length;
}

template <template <typename> class V, typename T>
inline V<T> NormalizeSafe(const V<T>& v)
{
    T length = v.Length();
    if (length < std::numeric_limits<T>::epsilon())
    {
        return T::zero;
    }

    T inv_length = T(1) / length;
    return v * inv_length;
}

template <typename T>
constexpr inline T Sqr(T v)
{
    return v * v;
}

inline Float SafeSqrt(Float x)
{
    return std::sqrt(std::max<Float>(0, x));
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

constexpr inline Float DegToRad(Float deg)
{
    return deg * pi / 180;
}

constexpr inline Float RadToDeg(Float rad)
{
    return rad * inv_pi * 180;
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
constexpr inline T Lerp(const T& start, const T& end, Float t)
{
    return start * (T(1) - t) + end * t;
}

template <typename T>
constexpr inline T Slerp(const T& start, const T& end, Float t)
{
    Float dot = Clamp(Dot(start, end), -1.0f, 1.0f);
    Float angle = std::acos(dot) * t;

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