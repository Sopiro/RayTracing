#pragma once

#include "constant_color.h"
#include "mesh.h"
#include "transform.h"

namespace bulbit
{

Ref<Microfacet> RandomMicrofacetMaterial();

Ref<Mesh> CreateRectXY(const Transform& transform,
                       const Ref<Material> material,
                       const Point2& texCoord = Point2(1, 1));
Ref<Mesh> CreateRectXZ(const Transform& transform,
                       const Ref<Material> material,
                       const Point2& texCoord = Point2(1, 1));
Ref<Mesh> CreateRectYZ(const Transform& transform,
                       const Ref<Material> material,
                       const Point2& texCoord = Point2(1, 1));
Ref<Mesh> CreateBox(const Transform& transform,
                    const Ref<Material> material,
                    const Point2& texCoord = Point2(1, 1));

inline bool IsNullish(Float v)
{
    return std::isnan(v) || std::isinf(v);
}

template <typename T>
inline bool IsNullish(const T& v)
{
    return v.IsNullish();
}

#define checkNull(v)                                                                                                             \
    if (IsNullish(v))                                                                                                            \
    {                                                                                                                            \
        std::cout << #v;                                                                                                         \
        std::cout << " null" << std::endl;                                                                                       \
    }

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline std::ostream& operator<<(std::ostream& out, const Mat3& m)
{
    // clang-format off
    return out << m.ex.x << ' ' << m.ey.x << ' ' << m.ez.x << '\n'
               << m.ex.y << ' ' << m.ey.y << ' ' << m.ez.y << '\n'
               << m.ex.z << ' ' << m.ey.z << ' ' << m.ez.z << '\n';
    // clang-format on
}

inline std::ostream& operator<<(std::ostream& out, const Mat4& m)
{
    // clang-format off
    return out << m.ex.x << ' ' << m.ey.x << ' ' << m.ez.x << ' ' << m.ew.x << '\n'
               << m.ex.y << ' ' << m.ey.y << ' ' << m.ez.y << ' ' << m.ew.y << '\n'
               << m.ex.z << ' ' << m.ey.z << ' ' << m.ez.z << ' ' << m.ew.z << '\n'
               << m.ex.w << ' ' << m.ey.w << ' ' << m.ez.w << ' ' << m.ew.w << '\n';

    // clang-format on
}

} // namespace bulbit
