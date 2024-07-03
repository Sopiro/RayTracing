#pragma once

#include "tuple.h"

namespace bulbit
{

template <typename T>
struct Vector2 : public Tuple2<Vector2, T>
{
    using Tuple2<Vector2, T>::x;
    using Tuple2<Vector2, T>::y;

    Vector2() = default;

    constexpr explicit Vector2(T v)
        : Tuple2<Vector2, T>(v, v)
    {
    }

    constexpr Vector2(T x, T y)
        : Tuple2<Vector2, T>(x, y)
    {
    }

    template <typename U>
    constexpr Vector2(Vector2<U> v)
        : Tuple2<Vector2, T>(T(v.x), T(v.y))
    {
    }

    void Negate()
    {
        x = -x;
        y = -y;
    }

    T Length2() const
    {
        return x * x + y * y;
    }

    T Length() const
    {
        return std::sqrt(Length2());
    }

    T Normalize()
    {
        T length = Length();
        if (length < std::numeric_limits<T>::epsilon())
        {
            return T(0);
        }

        T inv_length = T(1) / length;
        x *= inv_length;
        y *= inv_length;

        return length;
    }

    static const Vector2 zero;
};

template <typename T>
const inline Vector2<T> Vector2<T>::zero = Vector2<T>(T(0), T(0));

template <typename T>
struct Vector3 : public Tuple3<Vector3, T>
{
    using Tuple3<Vector3, T>::x;
    using Tuple3<Vector3, T>::y;
    using Tuple3<Vector3, T>::z;

    Vector3() = default;

    constexpr explicit Vector3(T v)
        : Tuple3<Vector3, T>(v, v, v)
    {
    }

    constexpr Vector3(T x, T y, T z)
        : Tuple3<Vector3, T>(x, y, z)
    {
    }

    template <typename U>
    constexpr Vector3(Vector3<U> v)
        : Tuple3<Vector3, T>(T(v.x), T(v.y), T(v.z))
    {
    }

    void Negate()
    {
        x = -x;
        y = -y;
        z = -z;
    }

    T Length2() const
    {
        return x * x + y * y + z * z;
    }

    T Length() const
    {
        return std::sqrt(Length2());
    }

    T Normalize()
    {
        T length = Length();
        if (length < std::numeric_limits<T>::epsilon())
        {
            return T(0);
        }

        T inv_length = T(1) / length;
        x *= inv_length;
        y *= inv_length;
        z *= inv_length;

        return length;
    }

    static const Vector3 zero;
};

template <typename T>
const inline Vector3<T> Vector3<T>::zero = Vector3<T>(T(0), T(0), T(0));

template <typename T>
struct Vector4 : public Tuple4<Vector4, T>
{
    using Tuple4<Vector4, T>::x;
    using Tuple4<Vector4, T>::y;
    using Tuple4<Vector4, T>::z;
    using Tuple4<Vector4, T>::w;

    Vector4() = default;

    constexpr explicit Vector4(T v)
        : Tuple4<Vector4, T>(v, v, v, v)
    {
    }

    constexpr Vector4(T x, T y, T z, T w)
        : Tuple4<Vector4, T>(x, y, z, w)
    {
    }

    template <typename U>
    constexpr Vector4(Vector3<U> v, T w)
        : Tuple4<Vector4, T>(T(v.x), T(v.y), T(v.z), w)
    {
    }

    template <typename U>
    constexpr Vector4(Vector4<U> v)
        : Tuple4<Vector4, T>(T(v.x), T(v.y), T(v.z), T(v.w))
    {
    }

    void Negate()
    {
        x = -x;
        y = -y;
        z = -z;
        w = -w;
    }

    T Length2() const
    {
        return x * x + y * y + z * z + w * w;
    }

    T Length() const
    {
        return std::sqrt(Length2());
    }

    T Normalize()
    {
        T length = Length();
        if (length < std::numeric_limits<T>::epsilon())
        {
            return T(0);
        }

        T inv_length = T(1) / length;
        x *= inv_length;
        y *= inv_length;
        z *= inv_length;
        w *= inv_length;

        return length;
    }

    static const Vector4 zero;
};

template <typename T>
const inline Vector4<T> Vector4<T>::zero = Vector4<T>(T(0), T(0), T(0), T(0));

// Vector2 inline functions begin

template <typename T>
inline Vector2<T> Min(const Vector2<T>& a, const Vector2<T>& b)
{
    return Vector2<T>(std::min(a.x, b.x), std::min(a.y, b.y));
}

template <typename T>
inline Vector2<T> Max(const Vector2<T>& a, const Vector2<T>& b)
{
    return Vector2<T>(std::max(a.x, b.x), std::max(a.y, b.y));
}

template <typename T>
inline T Dot(const Vector2<T>& a, const Vector2<T>& b)
{
    return a.x * b.x + a.y * b.y;
}

template <typename T>
inline T Cross(const Vector2<T>& a, const Vector2<T>& b)
{
    return a.x * b.y - a.y * b.x;
}

template <typename T>
inline Vector2<T> Cross(T s, const Vector2<T>& v)
{
    return Vector2<T>(-s * v.y, s * v.x);
}

template <typename T>
inline Vector2<T> Cross(const Vector2<T>& v, T s)
{
    return Vector2<T>(s * v.y, -s * v.x);
}

template <typename T>
inline Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b)
{
    return Vector2<T>(a.x + b.x, a.y + b.y);
}

template <typename T>
inline Vector2<T> operator+(const Vector2<T>& a, T b)
{
    return Vector2<T>(a.x + b, a.y + b);
}

template <typename T>
inline Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b)
{
    return Vector2<T>(a.x - b.x, a.y - b.y);
}

template <typename T>
inline Vector2<T> operator-(const Vector2<T>& a, T b)
{
    return Vector2<T>(a.x - b, a.y - b);
}

template <typename T>
inline Vector2<T> operator*(const Vector2<T>& v, T s)
{
    return Vector2<T>(v.x * s, v.y * s);
}

template <typename T>
inline Vector2<T> operator*(T s, const Vector2<T>& v)
{
    return Vector2<T>(v.x * s, v.y * s);
}

template <typename T>
inline Vector2<T> operator*(const Vector2<T>& a, const Vector2<T>& b)
{
    return Vector2<T>(a.x * b.x, a.y * b.y);
}

template <typename T>
inline Vector2<T> operator/(const Vector2<T>& v, T s)
{
    return Vector2<T>(v.x / s, v.y / s);
}

template <typename T>
inline Vector2<T> operator/(T s, const Vector2<T>& v)
{
    return Vector2<T>(s / v.x, s / v.y);
}

template <typename T>
inline bool operator==(const Vector2<T>& a, const Vector2<T>& b)
{
    return a.x == b.x && a.y == b.y;
}

template <typename T>
inline bool operator!=(const Vector2<T>& a, const Vector2<T>& b)
{
    return a.x != b.x || a.y != b.y;
}

template <typename T>
constexpr inline T Length2(const Vector2<T>& v)
{
    return v.x * v.x + v.y * v.y;
}

template <typename T>
constexpr inline T Length(const Vector2<T>& v)
{
    return std::sqrt(Length2(v));
}

template <typename T>
inline T Dist2(const Vector2<T>& a, const Vector2<T>& b)
{
    return Length2(a - b);
}

template <typename T>
inline T Dist(const Vector2<T>& a, const Vector2<T>& b)
{
    return Length(a - b);
}

// Vector2 inline functions end

// Vector3 inline functions begin

template <typename T>
inline Vector3<T> Min(const Vector3<T>& a, const Vector3<T>& b)
{
    return Vector3<T>(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

template <typename T>
inline Vector3<T> Max(const Vector3<T>& a, const Vector3<T>& b)
{
    return Vector3<T>(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

template <typename T>
inline T Dot(const Vector3<T>& a, const Vector3<T>& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
inline Vector3<T> Cross(const Vector3<T>& a, const Vector3<T>& b)
{
    return Vector3<T>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

template <typename T>
inline Vector3<T> operator+(const Vector3<T>& a, const Vector3<T>& b)
{
    return Vector3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template <typename T>
inline Vector3<T> operator+(const Vector3<T>& a, T b)
{
    return Vector3<T>(a.x + b, a.y + b, a.z + b);
}

template <typename T>
inline Vector3<T> operator-(const Vector3<T>& a, const Vector3<T>& b)
{
    return Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template <typename T>
inline Vector3<T> operator-(const Vector3<T>& a, T b)
{
    return Vector3<T>(a.x - b, a.y - b, a.z - b);
}

template <typename T>
inline Vector3<T> operator*(const Vector3<T>& v, T s)
{
    return Vector3<T>(v.x * s, v.y * s, v.z * s);
}

template <typename T>
inline Vector3<T> operator*(T s, const Vector3<T>& v)
{
    return Vector3<T>(v.x * s, v.y * s, v.z * s);
}

template <typename T>
inline Vector3<T> operator*(const Vector3<T>& a, const Vector3<T>& b)
{
    return Vector3<T>(a.x * b.x, a.y * b.y, a.z * b.z);
}

template <typename T>
inline Vector3<T> operator/(const Vector3<T>& v, T s)
{
    return Vector3<T>(v.x / s, v.y / s, v.z / s);
}

template <typename T>
inline Vector3<T> operator/(T s, const Vector3<T>& v)
{
    return Vector3<T>(s / v.x, s / v.y, s / v.z);
}

template <typename T>
inline Vector3<T> operator/(const Vector3<T>& a, const Vector3<T>& b)
{
    return Vector3<T>(a.x / b.x, a.y / b.y, a.z / b.z);
}

template <typename T>
inline bool operator==(const Vector3<T>& a, const Vector3<T>& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

template <typename T>
inline bool operator!=(const Vector3<T>& a, const Vector3<T>& b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z;
}

template <typename T>
constexpr inline T Length2(const Vector3<T>& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

template <typename T>
constexpr inline T Length(const Vector3<T>& v)
{
    return std::sqrt(Length2(v));
}

template <typename T>
inline T Dist2(const Vector3<T>& a, const Vector3<T>& b)
{
    return Length2(b - a);
}

template <typename T>
inline T Dist(const Vector3<T>& a, const Vector3<T>& b)
{
    return Length(b - a);
}

// Vector3 inline functions end

// Vector4 inline functions begin

template <typename T>
inline Vector4<T> Min(const Vector4<T>& a, const Vector4<T>& b)
{
    return Vector4<T>(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w));
}

template <typename T>
inline Vector4<T> Max(const Vector4<T>& a, const Vector4<T>& b)
{
    return Vector4<T>(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w));
}

template <typename T>
inline T Dot(const Vector4<T>& a, const Vector4<T>& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

template <typename T>
inline Vector4<T> operator+(const Vector4<T>& a, const Vector4<T>& b)
{
    return Vector4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

template <typename T>
inline Vector4<T> operator+(const Vector4<T>& a, T b)
{
    return Vector4<T>(a.x + b, a.y + b, a.z + b, a.w + b);
}

template <typename T>
inline Vector4<T> operator-(const Vector4<T>& a, const Vector4<T>& b)
{
    return Vector4<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

template <typename T>
inline Vector4<T> operator-(const Vector4<T>& a, T b)
{
    return Vector4<T>(a.x - b, a.y - b, a.z - b, a.w - b);
}

template <typename T>
inline Vector4<T> operator*(const Vector4<T>& v, T s)
{
    return Vector4<T>(v.x * s, v.y * s, v.z * s, v.w * s);
}

template <typename T>
inline Vector4<T> operator*(T s, const Vector4<T>& v)
{
    return Vector4<T>(v.x * s, v.y * s, v.z * s, v.w * s);
}

template <typename T>
inline Vector4<T> operator*(const Vector4<T>& a, const Vector4<T>& b)
{
    return Vector4<T>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

template <typename T>
inline Vector4<T> operator/(const Vector4<T>& v, T s)
{
    return Vector4<T>(v.x / s, v.y / s, v.z / s, v.w / s);
}

template <typename T>
inline Vector4<T> operator/(T s, const Vector4<T>& v)
{
    return Vector4<T>(s / v.x, s / v.y, s / v.z, s / v.w);
}

template <typename T>
inline Vector4<T> operator/(const Vector4<T>& a, const Vector4<T>& b)
{
    return Vector4<T>(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

template <typename T>
inline bool operator==(const Vector4<T>& a, const Vector4<T>& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

template <typename T>
inline bool operator!=(const Vector4<T>& a, const Vector4<T>& b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}

// Vector4 inline functions end

} // namespace bulbit