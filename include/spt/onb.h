#pragma once

#include "common.h"

namespace spt
{

// Orthonormal basis
struct ONB
{
    ONB() = default;
    explicit ONB(const Vec3& normal);

    Vec3& operator[](int32 i);
    Vec3 operator[](int32 i) const;

    Vec3 GetLocal(float64 x, float64 y, float64 z) const;
    Vec3 GetLocal(const Vec3& d) const;
    void BuildFromW(const Vec3& n);

    // tangent, bitangent, normal
    Vec3 u, v, w;
};

inline ONB::ONB(const Vec3& normal)
{
    BuildFromW(normal);
}

inline Vec3& ONB::operator[](int32 i)
{
    return (&u)[i];
}

inline Vec3 ONB::operator[](int32 i) const
{
    return (&u)[i];
}

inline Vec3 ONB::GetLocal(float64 x, float64 y, float64 z) const
{
    return x * u + y * v + z * w;
}

inline Vec3 ONB::GetLocal(const Vec3& d) const
{
    return d.x * u + d.y * v + d.z * w;
}

inline void ONB::BuildFromW(const Vec3& n)
{
    w = n.Normalized();

    Vec3 t = (fabs(w.y) > 0.999) ? x_axis : y_axis;

    u = Cross(t, w).Normalized();
    v = Cross(w, u);
}

} // namespace spt
