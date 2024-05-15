#pragma once

#include "math.h"

namespace bulbit
{

struct Transform
{
    Vec3 p; // position
    Quat q; // orientation
    Vec3 r; // scale

    Transform() = default;

    Transform(Identity)
        : p{ 0.0f }
        , q{ identity }
        , r{ 1.0f }
    {
    }

    Transform(const Vec3& position)
        : p{ position }
        , q{ identity }
        , r{ 1.0f }
    {
    }

    Transform(const Quat& orientation)
        : p{ 0.0f }
        , q{ orientation }
        , r{ 1.0f }
    {
    }

    Transform(const Vec3& position, const Quat& orientation)
        : p{ position }
        , q{ orientation }
        , r{ 1.0f }
    {
    }

    Transform(const Vec3& position, const Quat& orientation, const Vec3& scale)
        : p{ position }
        , q{ orientation }
        , r{ scale }
    {
    }

    Transform(Float x, Float y, Float z, const Quat& orientation = Quat(1.0f), const Vec3& scale = Vec3(1.0f))
        : p{ x, y, z }
        , q{ orientation }
        , r{ scale }
    {
    }

    void Set(const Vec3& position, const Quat& orientation, const Vec3& scale)
    {
        p = position;
        q = orientation;
        r = scale;
    }

    void SetIdentity()
    {
        p.SetZero();
        q.SetIdentity();
        r.Set(1.0f, 1.0f, 1.0f);
    }

    Transform& operator*=(const Transform& other);

    Transform GetInverse() const
    {
        return Transform{ q.RotateInv(-p), q.GetConjugate(), 1.0f / r };
    }
};

inline bool operator==(const Transform& a, const Transform& b)
{
    return a.p == b.p && a.q == b.q;
}

inline Vec3 operator*(const Transform& t, const Vec3& v)
{
    return t.q.Rotate(t.r * v) + t.p;
}

// A * V
inline Vec3 Mul(const Transform& t, const Vec3& v)
{
    return t.q.Rotate(t.r * v) + t.p;
}

// A^T * V
inline Vec3 MulT(const Transform& t, const Vec3& v)
{
    return t.q.RotateInv(1 / t.r * v - t.p);
}

inline Transform operator*(const Transform& a, const Transform& b)
{
    return Transform{ a.q.Rotate(b.p) + a.p, a.q * b.q, a.r * b.r };
}

// A * B
inline Transform Mul(const Transform& a, const Transform& b)
{
    return Transform{ a.q.Rotate(b.p) + a.p, a.q * b.q, a.r * b.r };
}

// A^T * B
inline Transform MulT(const Transform& a, const Transform& b)
{
    Quat invQ = a.q.GetConjugate();

    return Transform{ invQ.Rotate(b.p - a.p), invQ * b.q, (1.0f / a.r) * b.r };
}

inline Transform& Transform::operator*=(const Transform& other)
{
    *this = Mul(*this, other);
    return *this;
}

} // namespace bulbit