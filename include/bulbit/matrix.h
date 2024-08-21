#pragma once

#include "common.h"
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

    constexpr Mat2() = default;

    constexpr Mat2(Identity)
        : Mat2(1)
    {
    }

    constexpr explicit Mat2(Float v)
        : ex{ v, 0 }
        , ey{ 0, v }
    {
    }

    constexpr explicit Mat2(const Vec2& v)
        : ex{ v.x, 0 }
        , ey{ 0, v.y }
    {
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

    constexpr void SetIdentity()
    {
        // clang-format off
        ex.x = 1;    ey.x = 0;
        ex.y = 0;    ey.y = 1;
        // clang-format on
    }

    constexpr void SetZero()
    {
        // clang-format off
        ex.x = 0;    ey.x = 0;
        ex.y = 0;    ey.y = 0;
        // clang-format on
    }

    constexpr Mat2 GetTranspose() const
    {
        Mat2 t;

        // clang-format off
        t.ex.x = ex.x;    t.ey.x = ex.y;
        t.ex.y = ey.x;    t.ey.y = ey.y;
        // clang-format on

        return t;
    }

    constexpr Mat2 GetInverse() const
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

    constexpr Float GetDeterminant() const
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

    constexpr Mat3() = default;

    constexpr Mat3(Identity)
        : Mat3(1)
    {
    }

    constexpr explicit Mat3(Float v)
        : ex{ v, 0, 0 }
        , ey{ 0, v, 0 }
        , ez{ 0, 0, v }
    {
    }

    constexpr explicit Mat3(const Vec3& v)
        : ex{ v.x, 0, 0 }
        , ey{ 0, v.y, 0 }
        , ez{ 0, 0, v.z }
    {
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

    constexpr void SetIdentity()
    {
        // clang-format off
        ex.x = 1;    ey.x = 0;    ez.x = 0;
        ex.y = 0;    ey.y = 1;    ez.y = 0;
        ex.z = 0;    ey.z = 0;    ez.z = 1;
        // clang-format on
    }

    constexpr void SetZero()
    {
        // clang-format off
        ex.x = 0;    ey.x = 0;    ez.x = 0;
        ex.y = 0;    ey.y = 0;    ez.y = 0;
        ex.z = 0;    ey.z = 0;    ez.z = 0;
        // clang-format on
    }

    constexpr Mat3 GetTranspose() const
    {
        Mat3 t;

        // clang-format off
        t.ex.x = ex.x;    t.ey.x = ex.y;    t.ez.x = ex.z;
        t.ex.y = ey.x;    t.ey.y = ey.y;    t.ez.y = ey.z;
        t.ex.z = ez.x;    t.ey.z = ez.y;    t.ez.z = ez.z;
        // clang-format on

        return t;
    }

    constexpr Mat3 GetInverse() const;

    Mat3 Scale(const Vec2& scale) const;
    Mat3 Rotate(Float rotation) const;
    Mat3 Translate(const Vec2& translation) const;

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

    constexpr Mat4() = default;

    constexpr Mat4(Identity)
        : Mat4(1)
    {
    }

    constexpr explicit Mat4(Float v)
        : ex{ v, 0, 0, 0 }
        , ey{ 0, v, 0, 0 }
        , ez{ 0, 0, v, 0 }
        , ew{ 0, 0, 0, v }
    {
    }

    constexpr explicit Mat4(const Vec4& v)
        : ex{ v.x, 0, 0, 0 }
        , ey{ 0, v.y, 0, 0 }
        , ez{ 0, 0, v.z, 0 }
        , ew{ 0, 0, 0, v.w }
    {
    }

    constexpr Mat4(const Vec4& c1, const Vec4& c2, const Vec4& c3, const Vec4& c4)
        : ex{ c1 }
        , ey{ c2 }
        , ez{ c3 }
        , ew{ c4 }
    {
    }

    constexpr Mat4(const Mat3& r, const Vec3& p)
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

    constexpr void SetIdentity()
    {
        // clang-format off
        ex.x = 1;    ey.x = 0;    ez.x = 0;    ew.x = 0;
        ex.y = 0;    ey.y = 1;    ez.y = 0;    ew.y = 0;
        ex.z = 0;    ey.z = 0;    ez.z = 1;    ew.z = 0;
        ex.w = 0;    ey.w = 0;    ez.w = 0;    ew.w = 1;
        // clang-format on
    }

    constexpr void SetZero()
    {
        // clang-format off
        ex.x = 0;    ey.x = 0;    ez.x = 0;    ew.x = 0;
        ex.y = 0;    ey.y = 0;    ez.y = 0;    ew.y = 0;
        ex.z = 0;    ey.z = 0;    ez.z = 0;    ew.z = 0;
        ex.w = 0;    ey.w = 0;    ez.w = 0;    ew.w = 0;
        // clang-format on
    }

    constexpr Mat4 GetTranspose() const
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

    constexpr Mat4 GetInverse() const;

    Mat4 Scale(const Vec3& scale) const;
    Mat4 Rotate(const Vec3& euler_rotation) const;
    Mat4 Translate(const Vec3& translation) const;

    static Mat4 Orth(Float left, Float right, Float bottom, Float top, Float z_near, Float z_far);
    static Mat4 Perspective(Float vertical_fov, Float aspect_ratio, Float z_near, Float z_far);

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

constexpr inline Mat2 operator+(const Mat2& a, const Mat2& b)
{
    return Mat2(a.ex + b.ex, a.ey + b.ey);
}

// M * V
constexpr inline Vec2 Mul(const Mat2& m, const Vec2& v)
{
    return Vec2(m.ex.x * v.x + m.ey.x * v.y, m.ex.y * v.x + m.ey.y * v.y);
}

// M^T * V
constexpr inline Vec2 MulT(const Mat2& m, const Vec2& v)
{
    return Vec2(Dot(m.ex, v), Dot(m.ey, v));
}

// A * B
constexpr inline Mat2 Mul(const Mat2& a, const Mat2& b)
{
    return Mat2(Mul(a, b.ex), Mul(a, b.ey));
}

// A^T * B
constexpr inline Mat2 MulT(const Mat2& a, const Mat2& b)
{
    Vec2 c1(Dot(a.ex, b.ex), Dot(a.ey, b.ex));
    Vec2 c2(Dot(a.ex, b.ey), Dot(a.ey, b.ey));

    return Mat2(c1, c2);
}
// Mat2 functions end

// Mat3 functions begin

// M * V
constexpr inline Vec3 Mul(const Mat3& m, const Vec3& v)
{
    return Vec3{
        m.ex.x * v.x + m.ey.x * v.y + m.ez.x * v.z,
        m.ex.y * v.x + m.ey.y * v.y + m.ez.y * v.z,
        m.ex.z * v.x + m.ey.z * v.y + m.ez.z * v.z,
    };
}

// M^T * V
constexpr inline Vec3 MulT(const Mat3& m, const Vec3& v)
{
    return Vec3(Dot(m.ex, v), Dot(m.ey, v), Dot(m.ez, v));
}

// A * B
constexpr inline Mat3 Mul(const Mat3& a, const Mat3& b)
{
    return Mat3(Mul(a, b.ex), Mul(a, b.ey), Mul(a, b.ez));
}

// A^T * B
constexpr inline Mat3 MulT(const Mat3& a, const Mat3& b)
{
    Vec3 c1(Dot(a.ex, b.ex), Dot(a.ey, b.ex), Dot(a.ez, b.ex));
    Vec3 c2(Dot(a.ex, b.ey), Dot(a.ey, b.ey), Dot(a.ez, b.ey));
    Vec3 c3(Dot(a.ex, b.ez), Dot(a.ey, b.ez), Dot(a.ez, b.ez));

    return Mat3(c1, c2, c3);
}

constexpr inline Mat3 Mat3::GetInverse() const
{
    Mat3 t;

    Float det = ex.x * (ey.y * ez.z - ey.z * ez.y) - ey.x * (ex.y * ez.z - ez.y * ex.z) + ez.x * (ex.y * ey.z - ey.y * ex.z);

    if (det != 0)
    {
        det = 1 / det;
    }

    t.ex.x = (ey.y * ez.z - ey.z * ez.y) * det;
    t.ey.x = (ez.x * ey.z - ey.x * ez.z) * det;
    t.ez.x = (ey.x * ez.y - ez.x * ey.y) * det;
    t.ex.y = (ez.y * ex.z - ex.y * ez.z) * det;
    t.ey.y = (ex.x * ez.z - ez.x * ex.z) * det;
    t.ez.y = (ex.y * ez.x - ex.x * ez.y) * det;
    t.ex.z = (ex.y * ey.z - ex.z * ey.y) * det;
    t.ey.z = (ex.z * ey.x - ex.x * ey.z) * det;
    t.ez.z = (ex.x * ey.y - ex.y * ey.x) * det;

    return t;
}

inline Mat3 Mat3::Scale(const Vec2& scale) const
{
    Mat3 t{ identity };

    t.ex.x = scale.x;
    t.ey.y = scale.y;

    return Mul(*this, t);
}

inline Mat3 Mat3::Rotate(Float rotation) const
{
    Float s = std::sin(rotation);
    Float c = std::cos(rotation);

    Mat3 t;

    // clang-format off
        t.ex.x = c; t.ey.x = -s; t.ez.x = 0;
        t.ex.y = s; t.ey.y = c;  t.ez.y = 0;
        t.ex.z = 0; t.ey.z = 0;  t.ez.z = 1;
    // clang-format on

    return Mul(*this, t);
}

inline Mat3 Mat3::Translate(const Vec2& translation) const
{
    Mat3 t{ identity };

    t.ez.x = translation.x;
    t.ez.y = translation.y;

    return Mul(*this, t);
}

// Mat3 functions end

// Mat4 functions begin

// M * V
constexpr inline Vec4 Mul(const Mat4& m, const Vec4& v)
{
    return Vec4{
        m.ex.x * v.x + m.ey.x * v.y + m.ez.x * v.z + m.ew.x * v.w,
        m.ex.y * v.x + m.ey.y * v.y + m.ez.y * v.z + m.ew.y * v.w,
        m.ex.z * v.x + m.ey.z * v.y + m.ez.z * v.z + m.ew.z * v.w,
        m.ex.w * v.x + m.ey.w * v.y + m.ez.w * v.z + m.ew.w * v.w,
    };
}

// M^T * V
constexpr inline Vec4 MulT(const Mat4& m, const Vec4& v)
{
    return Vec4(Dot(m.ex, v), Dot(m.ey, v), Dot(m.ez, v), Dot(m.ew, v));
}

// A * B
constexpr inline Mat4 Mul(const Mat4& a, const Mat4& b)
{
    return Mat4(Mul(a, b.ex), Mul(a, b.ey), Mul(a, b.ez), Mul(a, b.ew));
}

// A^T * B
constexpr inline Mat4 MulT(const Mat4& a, const Mat4& b)
{
    Vec4 c1(Dot(a.ex, b.ex), Dot(a.ey, b.ex), Dot(a.ez, b.ex), Dot(a.ew, b.ex));
    Vec4 c2(Dot(a.ex, b.ey), Dot(a.ey, b.ey), Dot(a.ez, b.ey), Dot(a.ew, b.ey));
    Vec4 c3(Dot(a.ex, b.ez), Dot(a.ey, b.ez), Dot(a.ez, b.ez), Dot(a.ew, b.ez));
    Vec4 c4(Dot(a.ex, b.ew), Dot(a.ey, b.ew), Dot(a.ez, b.ew), Dot(a.ew, b.ew));

    return Mat4(c1, c2, c3, c4);
}

constexpr inline Mat4 Mat4::GetInverse() const
{
    Float a2323 = ez.z * ew.w - ez.w * ew.z;
    Float a1323 = ez.y * ew.w - ez.w * ew.y;
    Float a1223 = ez.y * ew.z - ez.z * ew.y;
    Float a0323 = ez.x * ew.w - ez.w * ew.x;
    Float a0223 = ez.x * ew.z - ez.z * ew.x;
    Float a0123 = ez.x * ew.y - ez.y * ew.x;
    Float a2313 = ey.z * ew.w - ey.w * ew.z;
    Float a1313 = ey.y * ew.w - ey.w * ew.y;
    Float a1213 = ey.y * ew.z - ey.z * ew.y;
    Float a2312 = ey.z * ez.w - ey.w * ez.z;
    Float a1312 = ey.y * ez.w - ey.w * ez.y;
    Float a1212 = ey.y * ez.z - ey.z * ez.y;
    Float a0313 = ey.x * ew.w - ey.w * ew.x;
    Float a0213 = ey.x * ew.z - ey.z * ew.x;
    Float a0312 = ey.x * ez.w - ey.w * ez.x;
    Float a0212 = ey.x * ez.z - ey.z * ez.x;
    Float a0113 = ey.x * ew.y - ey.y * ew.x;
    Float a0112 = ey.x * ez.y - ey.y * ez.x;

    Float det = ex.x * (ey.y * a2323 - ey.z * a1323 + ey.w * a1223) - ex.y * (ey.x * a2323 - ey.z * a0323 + ey.w * a0223) +
                ex.z * (ey.x * a1323 - ey.y * a0323 + ey.w * a0123) - ex.w * (ey.x * a1223 - ey.y * a0223 + ey.z * a0123);

    if (det != 0)
    {
        det = 1 / det;
    }

    Mat4 t;

    t.ex.x = det * (ey.y * a2323 - ey.z * a1323 + ey.w * a1223);
    t.ex.y = det * -(ex.y * a2323 - ex.z * a1323 + ex.w * a1223);
    t.ex.z = det * (ex.y * a2313 - ex.z * a1313 + ex.w * a1213);
    t.ex.w = det * -(ex.y * a2312 - ex.z * a1312 + ex.w * a1212);
    t.ey.x = det * -(ey.x * a2323 - ey.z * a0323 + ey.w * a0223);
    t.ey.y = det * (ex.x * a2323 - ex.z * a0323 + ex.w * a0223);
    t.ey.z = det * -(ex.x * a2313 - ex.z * a0313 + ex.w * a0213);
    t.ey.w = det * (ex.x * a2312 - ex.z * a0312 + ex.w * a0212);
    t.ez.x = det * (ey.x * a1323 - ey.y * a0323 + ey.w * a0123);
    t.ez.y = det * -(ex.x * a1323 - ex.y * a0323 + ex.w * a0123);
    t.ez.z = det * (ex.x * a1313 - ex.y * a0313 + ex.w * a0113);
    t.ez.w = det * -(ex.x * a1312 - ex.y * a0312 + ex.w * a0112);
    t.ew.x = det * -(ey.x * a1223 - ey.y * a0223 + ey.z * a0123);
    t.ew.y = det * (ex.x * a1223 - ex.y * a0223 + ex.z * a0123);
    t.ew.z = det * -(ex.x * a1213 - ex.y * a0213 + ex.z * a0113);
    t.ew.w = det * (ex.x * a1212 - ex.y * a0212 + ex.z * a0112);

    return t;
}

inline Mat4 Mat4::Scale(const Vec3& s) const
{
    Mat4 t{ identity };

    t.ex.x = s.x;
    t.ey.y = s.y;
    t.ez.z = s.z;

    return Mul(*this, t);
}

inline Mat4 Mat4::Rotate(const Vec3& r) const
{
    Float sinX = std::sin(r.x);
    Float cosX = std::cos(r.x);
    Float sinY = std::sin(r.y);
    Float cosY = std::cos(r.y);
    Float sinZ = std::sin(r.z);
    Float cosZ = std::cos(r.z);

    Mat4 t;

    t.ex.x = cosY * cosZ;
    t.ex.y = sinX * sinY * cosZ + cosX * sinZ;
    t.ex.z = -cosX * sinY * cosZ + sinX * sinZ;
    t.ex.w = 0;

    t.ey.x = -cosY * sinZ;
    t.ey.y = -sinX * sinY * sinZ + cosX * cosZ;
    t.ey.z = cosX * sinY * sinZ + sinX * cosZ;
    t.ey.w = 0;

    t.ez.x = sinY;
    t.ez.y = -sinX * cosY;
    t.ez.z = cosX * cosY;
    t.ez.w = 0;

    t.ew.x = 0;
    t.ew.y = 0;
    t.ew.z = 0;
    t.ew.w = 1;

    return Mul(*this, t);
}

inline Mat4 Mat4::Translate(const Vec3& v) const
{
    Mat4 t{ identity };

    t.ew.x = v.x;
    t.ew.y = v.y;
    t.ew.z = v.z;

    return Mul(*this, t);
}

inline Mat4 Mat4::Orth(Float left, Float right, Float bottom, Float top, Float z_near, Float z_far)
{
    Mat4 t{ identity };

    // Scale
    t.ex.x = 2 / (right - left);
    t.ey.y = 2 / (top - bottom);
    t.ez.z = 2 / (z_far - z_near);

    // Translation
    t.ew.x = -(right + left) / (right - left);
    t.ew.y = -(top + bottom) / (top - bottom);
    t.ew.z = -(z_far + z_near) / (z_far - z_near);

    return t;
}

inline Mat4 Mat4::Perspective(Float vertical_fov, Float aspect_ratio, Float z_near, Float z_far)
{
    Mat4 t{ identity };

    Float tan_half_fov = std::tan(vertical_fov / 2);

    // Scale
    t.ex.x = 1 / (aspect_ratio * tan_half_fov);
    t.ey.y = 1 / tan_half_fov;
    t.ez.z = -(z_far + z_near) / (z_far - z_near);
    t.ez.w = -1; // Needed for perspective division

    // Translation (for z-axis)
    t.ew.z = -(2 * z_far * z_near) / (z_far - z_near);

    // No translation in x or y
    t.ew.w = 0;

    return t;
}

// Mat4 functions end

} // namespace bulbit
