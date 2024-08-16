#pragma once

#include <format>

#include "vectors.h"

namespace bulbit
{

struct Quat;
struct Transform;

enum Identity
{
    identity
};

struct Mat2
{
    Vec2 ex, ey;

    Mat2() = default;

    Mat2(Identity)
        : Mat2(1)
    {
    }

    explicit Mat2(Float v)
    {
        // clang-format off
        ex.x = v;    ey.x = 0;
        ex.y = 0;    ey.y = v;
        // clang-format on
    }

    constexpr Mat2(const Vec2& c1, const Vec2& c2)
        : ex{ c1 }
        , ey{ c2 }
    {
    }

    Vec2& operator[](int32 i)
    {
        return (&ex)[i];
    }

    const Vec2& operator[](int32 i) const
    {
        return (&ex)[i];
    }

    void SetIdentity()
    {
        // clang-format off
        ex.x = 1;    ey.x = 0;
        ex.y = 0;    ey.y = 1;
        // clang-format on
    }

    void SetZero()
    {
        // clang-format off
        ex.x = 0;    ey.x = 0;
        ex.y = 0;    ey.y = 0;
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

        Float det = ex.x * ey.y - ey.x * ex.y;
        if (det != 0)
        {
            det = 1 / det;
        }

        t.ex.x = det * ey.y;
        t.ey.x = -det * ey.x;
        t.ex.y = -det * ex.y;
        t.ey.y = det * ex.x;

        return t;
    }

    Float GetDeterminant() const
    {
        return ex.x * ey.y - ey.x * ex.y;
    }

    std::string ToString() const
    {
        return std::format("{:.4f}\t{:.4f}\n{:.4f}\t{:.4f}", ex.x, ey.x, ex.y, ey.y);
    }
};

struct Mat3
{
    Vec3 ex, ey, ez;

    Mat3() = default;

    Mat3(Identity)
        : Mat3(1)
    {
    }

    explicit Mat3(Float v)
    {
        // clang-format off
        ex.x = v;    ey.x = 0;    ez.x = 0;
        ex.y = 0;    ey.y = v;    ez.y = 0;
        ex.z = 0;    ey.z = 0;    ez.z = v;
        // clang-format on
    }

    constexpr Mat3(const Vec3& c1, const Vec3& c2, const Vec3& c3)
        : ex{ c1 }
        , ey{ c2 }
        , ez{ c3 }
    {
    }

    Mat3(const Quat& q);

    Vec3& operator[](int32 i)
    {
        return (&ex)[i];
    }

    const Vec3& operator[](int32 i) const
    {
        return (&ex)[i];
    }

    void SetIdentity()
    {
        // clang-format off
        ex.x = 1;    ey.x = 0;    ez.x = 0;
        ex.y = 0;    ey.y = 1;    ez.y = 0;
        ex.z = 0;    ey.z = 0;    ez.z = 1;
        // clang-format on
    }

    void SetZero()
    {
        // clang-format off
        ex.x = 0;    ey.x = 0;    ez.x = 0;
        ex.y = 0;    ey.y = 0;    ez.y = 0;
        ex.z = 0;    ey.z = 0;    ez.z = 0;
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
    Mat3 Scale(Float x, Float y);
    Mat3 Rotate(Float z);
    Mat3 Translate(Float x, Float y);
    Mat3 Translate(const Vec2& v);

    std::string ToString() const
    {
        return std::format(
            "{:.4f}\t{:.4f}\t{:.4f}\n{:.4f}\t{:.4f}\t{:.4f}\n{:.4f}\t{:.4f}\t{:.4f}\n", ex.x, ey.x, ez.x, ex.y, ey.y, ez.y, ex.z,
            ey.z, ez.z
        );
    }
};

struct Mat4
{
    Vec4 ex, ey, ez, ew;

    Mat4() = default;

    Mat4(Identity)
        : Mat4(1)
    {
    }

    explicit Mat4(Float v)
    {
        // clang-format off
        ex.x = v;    ey.x = 0;    ez.x = 0;    ew.x = 0;
        ex.y = 0;    ey.y = v;    ez.y = 0;    ew.y = 0;
        ex.z = 0;    ey.z = 0;    ez.z = v;    ew.z = 0;
        ex.w = 0;    ey.w = 0;    ez.w = 0;    ew.w = v;
        // clang-format on
    }

    constexpr Mat4(const Vec4& c1, const Vec4& c2, const Vec4& c3, const Vec4& c4)
        : ex{ c1 }
        , ey{ c2 }
        , ez{ c3 }
        , ew{ c4 }
    {
    }

    Mat4(const Mat3& r, const Vec3& p)
        : ex{ r.ex, 0 }
        , ey{ r.ey, 0 }
        , ez{ r.ez, 0 }
        , ew{ p, 1 }
    {
    }

    Mat4(const Transform& t);

    Vec4& operator[](int32 i)
    {
        return (&ex)[i];
    }

    const Vec4& operator[](int32 i) const
    {
        return (&ex)[i];
    }

    void SetIdentity()
    {
        // clang-format off
        ex.x = 1;    ey.x = 0;    ez.x = 0;    ew.x = 0;
        ex.y = 0;    ey.y = 1;    ez.y = 0;    ew.y = 0;
        ex.z = 0;    ey.z = 0;    ez.z = 1;    ew.z = 0;
        ex.w = 0;    ey.w = 0;    ez.w = 0;    ew.w = 1;
        // clang-format on
    }

    void SetZero()
    {
        // clang-format off
        ex.x = 0;    ey.x = 0;    ez.x = 0;    ew.x = 0;
        ex.y = 0;    ey.y = 0;    ez.y = 0;    ew.y = 0;
        ex.z = 0;    ey.z = 0;    ez.z = 0;    ew.z = 0;
        ex.w = 0;    ey.w = 0;    ez.w = 0;    ew.w = 0;
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
    Mat4 Scale(Float x, Float y, Float z);
    Mat4 Rotate(Float x, Float y, Float z);
    Mat4 Translate(Float x, Float y, Float z);
    Mat4 Translate(const Vec3& v);

    std::string ToString() const
    {
        return std::format(
            "{:.4f}\t{:.4f}\t{:.4f}\t{:.4f}\n{:.4f}\t{:.4f}\t{:.4f}\t{:.4f}\n{:.4f}\t{:.4f}\t{:.4f}\t{:.4f}\n{:."
            "4f}\t{:.4f}\t{:.4f}\t{:.4f}\n",
            ex.x, ey.x, ez.x, ew.x, ex.y, ey.y, ez.y, ew.y, ex.z, ey.z, ez.z, ew.z, ex.w, ey.w, ez.w, ew.w
        );
    }
};

// Mat2 inline functions begin

inline Mat2 operator+(const Mat2& a, const Mat2& b)
{
    return Mat2(a.ex + b.ex, a.ey + b.ey);
}

// M * V
inline Vec2 Mul(const Mat2& m, const Vec2& v)
{
    return Vec2(m.ex.x * v.x + m.ey.x * v.y, m.ex.y * v.x + m.ey.y * v.y);
}

// M^T * V
inline Vec2 MulT(const Mat2& m, const Vec2& v)
{
    return Vec2(Dot(m.ex, v), Dot(m.ey, v));
}

// A * B
inline Mat2 Mul(const Mat2& a, const Mat2& b)
{
    return Mat2(Mul(a, b.ex), Mul(a, b.ey));
}

// A^T * B
inline Mat2 MulT(const Mat2& a, const Mat2& b)
{
    Vec2 c1(Dot(a.ex, b.ex), Dot(a.ey, b.ex));
    Vec2 c2(Dot(a.ex, b.ey), Dot(a.ey, b.ey));

    return Mat2(c1, c2);
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

// M^T * V
inline Vec3 MulT(const Mat3& m, const Vec3& v)
{
    return Vec3(Dot(m.ex, v), Dot(m.ey, v), Dot(m.ez, v));
}

// A * B
inline Mat3 Mul(const Mat3& a, const Mat3& b)
{
    return Mat3(Mul(a, b.ex), Mul(a, b.ey), Mul(a, b.ez));
}

// A^T * B
inline Mat3 MulT(const Mat3& a, const Mat3& b)
{
    Vec3 c1(Dot(a.ex, b.ex), Dot(a.ey, b.ex), Dot(a.ez, b.ex));
    Vec3 c2(Dot(a.ex, b.ey), Dot(a.ey, b.ey), Dot(a.ez, b.ey));
    Vec3 c3(Dot(a.ex, b.ez), Dot(a.ey, b.ez), Dot(a.ez, b.ez));

    return Mat3(c1, c2, c3);
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

// M^T * V
inline Vec4 MulT(const Mat4& m, const Vec4& v)
{
    return Vec4(Dot(m.ex, v), Dot(m.ey, v), Dot(m.ez, v), Dot(m.ew, v));
}

// A * B
inline Mat4 Mul(const Mat4& a, const Mat4& b)
{
    return Mat4(Mul(a, b.ex), Mul(a, b.ey), Mul(a, b.ez), Mul(a, b.ew));
}

// A^T * B
inline Mat4 MulT(const Mat4& a, const Mat4& b)
{
    Vec4 c1(Dot(a.ex, b.ex), Dot(a.ey, b.ex), Dot(a.ez, b.ex), Dot(a.ew, b.ex));
    Vec4 c2(Dot(a.ex, b.ey), Dot(a.ey, b.ey), Dot(a.ez, b.ey), Dot(a.ew, b.ey));
    Vec4 c3(Dot(a.ex, b.ez), Dot(a.ey, b.ez), Dot(a.ez, b.ez), Dot(a.ew, b.ez));
    Vec4 c4(Dot(a.ex, b.ew), Dot(a.ey, b.ew), Dot(a.ez, b.ew), Dot(a.ew, b.ew));

    return Mat4(c1, c2, c3, c4);
}

inline Mat4 Orth(Float left, Float right, Float bottom, Float top, Float zNear, Float zFar)
{
    Mat4 t{ identity };

    // Scale
    t.ex.x = 2 / (right - left);
    t.ey.y = 2 / (top - bottom);
    t.ez.z = 2 / (zFar - zNear);

    // Translation
    t.ew.x = -(right + left) / (right - left);
    t.ew.y = -(top + bottom) / (top - bottom);
    t.ew.z = -(zFar + zNear) / (zFar - zNear);

    return t;
}

// Mat4 functions end

} // namespace bulbit
