#pragma once

#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "math.h"
#include "types.h"

typedef Vec3 Color;
typedef Vec2 UV;

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
    return out << v.x << ' ' << v.y << ' ' << v.z << '\n';
}