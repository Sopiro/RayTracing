#pragma once

#include "common.h"
#include "spectrum.h"

namespace bulbit
{

class Texture
{
public:
    virtual ~Texture() = default;
    virtual Spectrum Evaluate(const Point2& uv) const = 0;
};

} // namespace bulbit