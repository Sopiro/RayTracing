#pragma once

#include "quaternion.h"
#include "vectors.h"

namespace bulbit
{

struct Transform
{
    Vec3 p; // position
    Quat q; // orientation
    Vec3 s; // scale

    constexpr Transform() = default;

    constexpr Transform(Identity)
        : p{ 0 }
        , q{ identity }
        , s{ 1 }
    {
    }

    constexpr Transform(const Vec3& position)
        : p{ position }
        , q{ identity }
        , s{ 1 }
    {
    }

    constexpr Transform(const Quat& orientation)
        : p{ 0 }
        , q{ orientation }
        , s{ 1 }
    {
    }

    constexpr Transform(const Vec3& position, const Quat& orientation)
        : p{ position }
        , q{ orientation }
        , s{ 1 }
    {
    }

    constexpr Transform(const Vec3& position, const Quat& orientation, const Vec3& scale)
        : p{ position }
        , q{ orientation }
        , s{ scale }
    {
    }

    constexpr Transform(Float x, Float y, Float z, const Quat& orientation = Quat(1), const Vec3& scale = Vec3(1))
        : p{ x, y, z }
        , q{ orientation }
        , s{ scale }
    {
    }

    Transform(const Mat4& m)
    {
        s.x = std::sqrt(m[0][0] * m[0][0] + m[1][0] * m[1][0] + m[2][0] * m[2][0]);
        s.y = std::sqrt(m[0][1] * m[0][1] + m[1][1] * m[1][1] + m[2][1] * m[2][1]);
        s.z = std::sqrt(m[0][2] * m[0][2] + m[1][2] * m[1][2] + m[2][2] * m[2][2]);

        q = Mat3(
            Vec3(m[0][0], m[0][1], m[0][2]) / s.x, Vec3(m[1][0], m[1][1], m[1][2]) / s.y, Vec3(m[2][0], m[2][1], m[2][2]) / s.z
        );

        p.x = m[3][0];
        p.y = m[3][1];
        p.z = m[3][2];
    }

    constexpr void Set(const Vec3& position, const Quat& orientation, const Vec3& scale)
    {
        p = position;
        q = orientation;
        s = scale;
    }

    constexpr void SetIdentity()
    {
        p.SetZero();
        q.SetIdentity();
        s.Set(1, 1, 1);
    }

    constexpr Transform& operator*=(const Transform& other);

    constexpr Transform GetInverse() const
    {
        return Transform{ q.RotateInv(-p), q.GetConjugate(), 1 / s };
    }
};

constexpr inline bool operator==(const Transform& a, const Transform& b)
{
    return a.p == b.p && a.q == b.q;
}

constexpr inline Vec3 operator*(const Transform& t, const Vec3& v)
{
    return t.q.Rotate(t.s * v) + t.p;
}

// A * V
constexpr inline Vec3 Mul(const Transform& t, const Vec3& v)
{
    return t.q.Rotate(t.s * v) + t.p;
}

// A^T * V
constexpr inline Vec3 MulT(const Transform& t, const Vec3& v)
{
    return t.q.RotateInv(Float(1) / t.s * v - t.p);
}

constexpr inline Transform operator*(const Transform& a, const Transform& b)
{
    return Transform{ a.q.Rotate(b.p) + a.p, a.q * b.q, a.s * b.s };
}

// A * B
constexpr inline Transform Mul(const Transform& a, const Transform& b)
{
    return Transform{ a.q.Rotate(b.p) + a.p, a.q * b.q, a.s * b.s };
}

// A^T * B
constexpr inline Transform MulT(const Transform& a, const Transform& b)
{
    Quat invQ = a.q.GetConjugate();

    return Transform{ invQ.Rotate(b.p - a.p), invQ * b.q, (1 / a.s) * b.s };
}

constexpr inline Transform& Transform::operator*=(const Transform& other)
{
    *this = Mul(*this, other);
    return *this;
}

} // namespace bulbit