#pragma once

#include "spectrum.h"

namespace bulbit
{

template <typename T>
struct Texture
{
    virtual ~Texture() = default;
    virtual T Evaluate(const Point2& uv) const = 0;
};

using FloatTexture = Texture<Float>;
using SpectrumTexture = Texture<Spectrum>;

enum TexCoordFilter
{
    repeat,
    clamp,
};

inline void FilterTexCoord(int32* u, int32* v, int32 width, int32 height, TexCoordFilter texcoord_filter)
{
    switch (texcoord_filter)
    {
    case TexCoordFilter::repeat:
    {
        *u = *u >= 0 ? *u % width : width - (-(*u) % width) - 1;
        *v = *v >= 0 ? *v % height : height - (-(*v) % height) - 1;
    }
    break;
    case TexCoordFilter::clamp:
    {
        *u = Clamp(*u, 0, width - 1);
        *v = Clamp(*v, 0, height - 1);
    }
    break;
    default:
        BulbitAssert(false);
        break;
    }
}

} // namespace bulbit