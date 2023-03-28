#pragma once

#include "common.h"

namespace spt
{

// Orthonormal basis
struct ONB
{
    ONB() = default;

    explicit ONB(const Vec3& normal)
    {
        BuildFromW(normal);
    }

    Vec3& operator[](int32 i)
    {
        return (&u)[i];
    }

    Vec3 operator[](int32 i) const
    {
        return (&u)[i];
    }

    Vec3 GetLocal(double x, double y, double z) const
    {
        return x * u + y * v + z * w;
    }

    Vec3 GetLocal(const Vec3& a) const
    {
        return a.x * u + a.y * v + a.z * w;
    }

    void BuildFromW(const Vec3& n)
    {
        w = n.Normalized();

        Vec3 a = (fabs(w.x) > 0.9) ? y_axis : x_axis;

        v = Cross(w, a).Normalized();
        u = Cross(w, v);
    }

    // tangent, bitangent, normal
    Vec3 u, v, w;
};

} // namespace spt
