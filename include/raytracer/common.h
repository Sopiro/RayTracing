#pragma once

#include <assert.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "math.h"
#include "types.h"

namespace spt
{

constexpr double ray_tolerance = 0.00001;

typedef Vec3 Color;
typedef Vec2 UV;

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
    return out << v.x << ' ' << v.y << ' ' << v.z << '\n';
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

inline Mat4 Convert(const aiMatrix4x4& aiMat)
{
    Mat4 t;
    t.ex.Set(aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1);
    t.ey.Set(aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2);
    t.ez.Set(aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3);
    t.ew.Set(aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4);

    return t;
}

} // namespace spt