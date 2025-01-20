#pragma once

#include "spectrum.h"

namespace bulbit
{

template <typename T>
class Texture
{
public:
    virtual ~Texture() = default;

    virtual T Evaluate(const Point2& uv) const = 0;
};

using FloatTexture = Texture<Float>;
using SpectrumTexture = Texture<Spectrum>;

} // namespace bulbit