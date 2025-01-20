#include "bulbit/textures.h"

namespace bulbit
{

ColorCheckerTexture::ColorCheckerTexture(const SpectrumTexture* a, const SpectrumTexture* b, const Point2& resolution)
    : a{ a }
    , b{ b }
    , resolution{ resolution }
{
}

ColorCheckerTexture::ColorCheckerTexture(
    std::pair<const SpectrumTexture*, const SpectrumTexture*> checker, const Point2& resolution
)
    : a{ checker.first }
    , b{ checker.second }
    , resolution{ resolution }
{
}

Spectrum ColorCheckerTexture::Evaluate(const Point2& uv) const
{
    Point2i scale = uv * resolution;
    int32 c = scale.x + scale.y;

    if (c % 2)
    {
        return a->Evaluate(uv);
    }
    else
    {
        return b->Evaluate(uv);
    }
}

FloatCheckerTexture::FloatCheckerTexture(const FloatTexture* a, const FloatTexture* b, const Point2& resolution)
    : a{ a }
    , b{ b }
    , resolution{ resolution }
{
}

FloatCheckerTexture::FloatCheckerTexture(std::pair<const FloatTexture*, const FloatTexture*> checker, const Point2& resolution)
    : a{ checker.first }
    , b{ checker.second }
    , resolution{ resolution }
{
}

Float FloatCheckerTexture::Evaluate(const Point2& uv) const
{
    Point2i scale = uv * resolution;
    int32 c = scale.x + scale.y;

    if (c % 2)
    {
        return a->Evaluate(uv);
    }
    else
    {
        return b->Evaluate(uv);
    }
}

} // namespace bulbit
