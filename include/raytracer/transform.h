#pragma once

#include "math.h"

struct Transform
{
    Vec3 p; // position
    Quat q; // orientation

    Transform() = default;

    Transform(Identity)
        : p{ precision(0.0) }
        , q{ identity }
    {
    }

    Transform(const Vec3& position)
        : p{ position }
        , q{ precision(1.0) }
    {
    }

    Transform(const Quat& orientation)
        : p{ precision(0.0) }
        , q{ orientation }
    {
    }

    Transform(const Vec3& position, const Quat& orientation)
        : p{ position }
        , q{ orientation }
    {
    }

    Transform(precision x, precision y, precision z, const Quat& orientation = Quat{ 1.0 })
        : p{ x, y, z }
        , q{ orientation }
    {
    }

    void Set(const Vec3& position, const Quat& orientation)
    {
        p = position;
        q = orientation;
    }

    void SetIdentity()
    {
        p.SetZero();
        q.SetIdentity();
    }

    Transform& operator*=(const Transform& other);

    Transform GetInverse() const
    {
        return Transform{ q.RotateInv(-p), q.GetConjugate() };
    }
};

inline bool operator==(const Transform& a, const Transform& b)
{
    return a.p == b.p && a.q == b.q;
}

inline Vec3 operator*(const Transform& t, const Vec3& v)
{
    return t.q.Rotate(v) + t.p;
}

inline Vec3 Mul(const Transform& t, const Vec3& v)
{
    return t.q.Rotate(v) + t.p;
}

inline Vec3 MulT(const Transform& t, const Vec3& v)
{
    return t.q.RotateInv(v - t.p);
}

inline Transform operator*(const Transform& a, const Transform& b)
{
    return Transform{ a.q.Rotate(b.p) + a.p, a.q * b.q };
}

inline Transform Mul(const Transform& a, const Transform& b)
{
    return Transform{ a.q.Rotate(b.p) + a.p, a.q * b.q };
}

inline Transform MulT(const Transform& a, const Transform& b)
{
    Quat invQ = a.q.GetConjugate();

    return Transform(invQ.Rotate(b.p - a.p), invQ * b.q);
}

inline Transform& Transform::operator*=(const Transform& other)
{
    *this = *this * other;
    return *this;
}