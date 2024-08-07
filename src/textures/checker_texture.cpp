#include "bulbit/textures.h"

namespace bulbit
{

CheckerTexture::CheckerTexture(const SpectrumTexture* a, const SpectrumTexture* b, const Point2& resolution)
    : a{ a }
    , b{ b }
    , resolution{ resolution }
{
}

CheckerTexture::CheckerTexture(std::pair<const SpectrumTexture*, const SpectrumTexture*> checker, const Point2& resolution)
    : a{ checker.first }
    , b{ checker.second }
    , resolution{ resolution }
{
}

Spectrum CheckerTexture::Evaluate(const Point2& uv) const
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

Float CheckerTexture::EvaluateAlpha(const Point2& uv) const
{
    Point2i scale = uv * resolution;
    int32 c = scale.x + scale.y;

    if (c % 2)
    {
        return a->EvaluateAlpha(uv);
    }
    else
    {
        return b->EvaluateAlpha(uv);
    }
}

} // namespace bulbit
