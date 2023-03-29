#pragma once

#include "math.h"

namespace spt
{

struct Transform
{
    Vec3 p; // position
    Quat q; // orientation
    Vec3 r; // scale

    Transform() = default;

    Transform(Identity)
        : p{ Real(0.0) }
        , q{ identity }
        , r{ Real(1.0) }
    {
    }

    Transform(const Vec3& position)
        : p{ position }
        , q{ identity }
        , r{ Real(1.0) }
    {
    }

    Transform(const Quat& orientation)
        : p{ Real(0.0) }
        , q{ orientation }
        , r{ Real(1.0) }
    {
    }

    Transform(const Vec3& position, const Quat& orientation)
        : p{ position }
        , q{ orientation }
        , r{ Real(1.0) }
    {
    }

    Transform(const Vec3& position, const Quat& orientation, const Vec3& scale)
        : p{ position }
        , q{ orientation }
        , r{ scale }
    {
    }

    Transform(Real x, Real y, Real z, const Quat& orientation = Quat{ Real(1.0) }, const Vec3& scale = Vec3{ Real{ 1.0 } })
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
        r.Set(Real(1.0), Real(1.0), Real(1.0));
    }

    Transform& operator*=(const Transform& other);

    Transform GetInverse() const
    {
        return Transform{ q.RotateInv(-p), q.GetConjugate(), Real(1.0) / r };
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
    return t.q.RotateInv(Real(1.0) / t.r * v - t.p);
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

    return Transform{ invQ.Rotate(b.p - a.p), invQ * b.q, (Real(1.0) / a.r) * b.r };
}

inline Transform& Transform::operator*=(const Transform& other)
{
    *this = Mul(*this, other);
    return *this;
}

} // namespace spt