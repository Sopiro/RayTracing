#pragma once

#include "constant_color.h"
#include "mesh.h"
#include "transform.h"

namespace bulbit
{

class Microfacet;
class Scene;

void CreateRectXY(Scene& scene, const Transform& transform, const Material* material, const Point2& texCoord = Point2(1, 1));
void CreateRectXZ(Scene& scene, const Transform& transform, const Material* material, const Point2& texCoord = Point2(1, 1));
void CreateRectYZ(Scene& scene, const Transform& transform, const Material* material, const Point2& texCoord = Point2(1, 1));
void CreateBox(Scene& scene, const Transform& transform, const Material* material, const Point2& texCoord = Point2(1, 1));

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

} // namespace bulbit
