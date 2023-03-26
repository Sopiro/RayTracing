#pragma once

#include "common.h"

namespace spt
{

class Texture
{
public:
    virtual Color Value(const UV& uv, const Vec3& p) const = 0;
};

} // namespace spt