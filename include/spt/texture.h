#pragma once

#include "common.h"
#include "spectrum.h"

namespace spt
{

class Texture
{
public:
    virtual ~Texture() = default;
    virtual Spectrum Evaluate(const Point2& uv) const = 0;
};

} // namespace spt