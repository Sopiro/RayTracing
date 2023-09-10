#pragma once

#include "common.h"

namespace spt
{

struct DirectionalLight
{
    Vec3 dir;
    Vec3 radiance;
    f64 radius;
};

} // namespace spt
