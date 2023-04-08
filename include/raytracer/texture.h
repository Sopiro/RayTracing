#pragma once

#include "common.h"

namespace spt
{

class Texture
{
public:
    virtual Color Value(const UV& uv, const Point& p) const = 0;
};

} // namespace spt