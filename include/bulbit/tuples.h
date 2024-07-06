#pragma once

#include "floats.h"

namespace bulbit
{

template <template <typename> class Child, typename T>
struct Tuple2
{
    static const int32 dimensions = 2;

    Tuple2() = default;

    constexpr Tuple2(T x, T y)
        : x(x)
        , y(y)
    {
        // assert(!IsNullish());
    }

    bool IsNullish() const
    {
        return bulbit::IsNullish(x) || bulbit::IsNullish(y);
    }

    T operator[](int32 i) const
    {
        assert(i >= 0 && i <= 1);
        return (&x)[i];
    }

    T& operator[](int32 i)
    {
        assert(i >= 0 && i <= 1);
        return (&x)[i];
    }

    bool operator==(Child<T> c) const
    {
        return x == c.x && y == c.y;
    }

    bool operator!=(Child<T> c) const
    {
        return x != c.x || y != c.y;
    }

    Child<T> operator-() const
    {
        return { -x, -y };
    }

    template <typename U>
    auto operator+(Child<U> c) const -> Child<decltype(T{} + U{})>
    {
        assert(!c.IsNullish());
        return { x + c.x, y + c.y };
    }

    template <typename U>
    auto operator-(Child<U> c) const -> Child<decltype(T{} - U{})>
    {
        assert(!c.IsNullish());
        return { x - c.x, y - c.y };
    }

    template <typename U>
    auto operator*(U s) const -> Child<decltype(T{} * U{})>
    {
        return { s * x, s * y };
    }

    template <typename U>
    auto operator/(U d) const -> Child<decltype(T{} / U{})>
    {
        assert(d != 0 && !bulbit::IsNullish(d));
        return { x / d, y / d };
    }

    template <typename U>
    Child<T>& operator+=(Child<U> c)
    {
        assert(!c.IsNullish());
        x += c.x;
        y += c.y;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    Child<T>& operator-=(Child<U> c)
    {
        assert(!c.IsNullish());
        x -= c.x;
        y -= c.y;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    Child<T>& operator*=(U s)
    {
        assert(!bulbit::IsNullish(s));
        x *= s;
        y *= s;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    Child<T>& operator/=(U d)
    {
        assert(d != 0);
        assert(!IsNullish(d));
        x /= d;
        y /= d;
        return static_cast<Child<T>&>(*this);
    }

    void SetZero()
    {
        x = T(0);
        y = T(0);
    }

    void Set(T nx, T ny)
    {
        x = nx;
        y = ny;
    }

    std::string ToString() const
    {
        return std::format("{:.4f}\t{:.4f}", x, y);
    }

    T x{}, y{};
};

template <template <typename> class Child, typename T>
struct Tuple3
{
    static const int32 dimensions = 3;

    Tuple3() = default;

    constexpr Tuple3(T x, T y, T z)
        : x(x)
        , y(y)
        , z(z)
    {
        // assert(!IsNullish());
    }

    bool IsNullish() const
    {
        return IsNullish(x) || IsNullish(y) || IsNullish(z);
    }

    T operator[](int32 i) const
    {
        assert(i >= 0 && i <= 2);
        return (&x)[i];
    }

    T& operator[](int32 i)
    {
        assert(i >= 0 && i <= 2);
        return (&x)[i];
    }

    bool operator==(Child<T> c) const
    {
        return x == c.x && y == c.y && z == c.z;
    }

    bool operator!=(Child<T> c) const
    {
        return x != c.x || y != c.y || z != c.z;
    }

    Child<T> operator-() const
    {
        return { -x, -y, -z };
    }

    template <typename U>
    auto operator+(Child<U> c) const -> Child<decltype(T{} + U{})>
    {
        assert(!c.IsNullish());
        return { x + c.x, y + c.y, z + c.z };
    }

    template <typename U>
    auto operator-(Child<U> c) const -> Child<decltype(T{} - U{})>
    {
        assert(!c.IsNullish());
        return { x - c.x, y - c.y, z - c.z };
    }

    template <typename U>
    auto operator*(U s) const -> Child<decltype(T{} * U{})>
    {
        return { s * x, s * y, s * z };
    }

    template <typename U>
    auto operator/(U d) const -> Child<decltype(T{} / U{})>
    {
        assert(d != 0);
        return { x / d, y / d, z / d };
    }

    template <typename U>
    Child<T>& operator+=(Child<U> c)
    {
        assert(!c.IsNullish());
        x += c.x;
        y += c.y;
        z += c.z;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    Child<T>& operator-=(Child<U> c)
    {
        assert(!c.IsNullish());
        x -= c.x;
        y -= c.y;
        z -= c.z;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    Child<T>& operator*=(U s)
    {
        assert(!bulbit::IsNullish(s));
        x *= s;
        y *= s;
        z *= s;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    Child<T>& operator/=(U d)
    {
        assert(d != 0);
        x /= d;
        y /= d;
        z /= d;
        return static_cast<Child<T>&>(*this);
    }

    void SetZero()
    {
        x = T(0);
        y = T(0);
        z = T(0);
    }

    void Set(T nx, T ny, T nz)
    {
        x = nx;
        y = ny;
        z = nz;
    }

    std::string ToString() const
    {
        return std::format("{:.4f}\t{:.4f}\t{:.4f}", x, y, z);
    }

    T x{}, y{}, z{};
};

template <template <typename> class Child, typename T>
struct Tuple4
{
    static const int32 dimensions = 4;

    Tuple4() = default;

    constexpr Tuple4(T x, T y, T z, T w)
        : x(x)
        , y(y)
        , z(z)
        , w(w)
    {
        // assert(!IsNullish());
    }

    bool IsNullish() const
    {
        return bulbit::IsNullish(x) || bulbit::IsNullish(y) || bulbit::IsNullish(z) || bulbit::IsNullish(w);
    }

    T operator[](int32 i) const
    {
        assert(i >= 0 && i <= 3);
        return (&x)[i];
    }

    T& operator[](int32 i)
    {
        assert(i >= 0 && i <= 3);
        return (&x)[i];
    }

    bool operator==(Child<T> c) const
    {
        return x == c.x && y == c.y && z == c.z || w == c.w;
    }

    bool operator!=(Child<T> c) const
    {
        return x != c.x || y != c.y || z != c.z || w != c.w;
    }

    Child<T> operator-() const
    {
        return { -x, -y, -z, -w };
    }

    template <typename U>
    auto operator+(Child<U> c) const -> Child<decltype(T{} + U{})>
    {
        assert(!c.IsNullish());
        return { x + c.x, y + c.y, z + c.z, w + c.w };
    }

    template <typename U>
    auto operator-(Child<U> c) const -> Child<decltype(T{} - U{})>
    {
        assert(!c.IsNullish());
        return { x - c.x, y - c.y, z - c.z, z - c.w };
    }

    template <typename U>
    auto operator*(U s) const -> Child<decltype(T{} * U{})>
    {
        return { s * x, s * y, s * z, s * w };
    }

    template <typename U>
    auto operator/(U d) const -> Child<decltype(T{} / U{})>
    {
        assert(d != 0);
        return { x / d, y / d, z / d, w / d };
    }

    template <typename U>
    Child<T>& operator+=(Child<U> c)
    {
        assert(!c.IsNullish());
        x += c.x;
        y += c.y;
        z += c.z;
        w += c.w;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    Child<T>& operator-=(Child<U> c)
    {
        assert(!c.IsNullish());
        x -= c.x;
        y -= c.y;
        z -= c.z;
        w -= c.w;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    Child<T>& operator*=(U s)
    {
        assert(!bulbit::IsNullish(s));
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    Child<T>& operator/=(U d)
    {
        assert(d != 0);
        x /= d;
        y /= d;
        z /= d;
        w /= d;
        return static_cast<Child<T>&>(*this);
    }

    void SetZero()
    {
        x = T(0);
        y = T(0);
        z = T(0);
        w = T(0);
    }

    void Set(T nx, T ny, T nz, T nw)
    {
        x = nx;
        y = ny;
        z = nz;
        w = nw;
    }

    std::string ToString() const
    {
        return std::format("{:.4f}\t{:.4f}\t{:.4f}\t{:.4f}", x, y, z, w);
    }

    T x{}, y{}, z{}, w{};
};

} // namespace bulbit
