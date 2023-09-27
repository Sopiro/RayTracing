#pragma once

#include "common.h"

namespace spt
{

// Orthonormal basis
// Only concern about polar angle
struct ONB
{
    ONB() = default;
    explicit ONB(const Vec3& normal);

    Vec3& operator[](int32 i);
    Vec3 operator[](int32 i) const;

    Vec3 GetLocal(Float x, Float y, Float z) const;
    Vec3 GetLocal(const Vec3& d) const;
    void BuildFromW(const Vec3& w);

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

inline Vec3 ONB::GetLocal(Float x, Float y, Float z) const
{
    return x * u + y * v + z * w;
}

inline Vec3 ONB::GetLocal(const Vec3& d) const
{
    return d.x * u + d.y * v + d.z * w;
}

inline void ONB::BuildFromW(const Vec3& n)
{
    w = n;

    Vec3 t = (std::fabs(w.y) > 0.999) ? x_axis : y_axis;

    u = Normalize(Cross(t, w));
    v = Cross(w, u);
}

} // namespace spt
