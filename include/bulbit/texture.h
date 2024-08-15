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

class FloatTexture : public Texture<Float>
{
public:
    virtual ~FloatTexture() = default;
};

class SpectrumTexture : public Texture<Spectrum>
{
public:
    virtual ~SpectrumTexture() = default;

    virtual Float EvaluateAlpha(const Point2& uv) const
    {
        BulbitNotUsed(uv);
        return 1;
    };
};

} // namespace bulbit